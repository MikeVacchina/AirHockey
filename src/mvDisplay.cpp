#include "mvDisplay.h"


mvDisplay::mvDisplay()
{
	//set default values
	width = 640;
	height = 480;
	
	lightOne = glm::vec4(10.0,0.0,0.0,1.0);
	lightTwo = glm::vec4(-10.0,5.0,5.0,1.0);

	AP = glm::vec4(0.2,0.2,0.2,1.0);
	DP = glm::vec4(0.7,0.7,0.7,1.0);
	SP = glm::vec4(0.0,0.0,0.0,1.0);

	shininess = 100.0;

    score_p1=0;
    score_p2=0;
}


mvDisplay::~mvDisplay()
{
}

void mvDisplay::initializeDisplay(std::string windowName, int w, int h)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
	width = w;
	height = h;

    // Name and create the Window
    glutCreateWindow(windowName.c_str());
}

bool mvDisplay::loadObjects()
{
	puck.assimpLoadMesh("Puck.obj");
	puck.setColor(1,0,0);
	table.assimpLoadMesh("TableTest.obj");
	table.loadBmpTexture("RinkIceTexture.bmp");
	//table.setColor(1,1,0);
	paddle1.assimpLoadMesh("Paddle1.obj");
	paddle1.setColor(0,1,0);
	paddle1.fixMesh(PADDLE1);
	paddle1.pos = glm::vec3(0.0,0.0,-5.5);
	paddle2.assimpLoadMesh("Paddle2.obj");
	paddle2.setColor(0,0,1);
	paddle2.fixMesh(PADDLE2);
	paddle2.pos = glm::vec3(0.0,0.0,5.5);

	return true;
}

bool mvDisplay::initializeDisplayResources()
{
	//initialize objects
	objectBufferInit(puck);
	objectBufferInit(paddle1);
	objectBufferInit(paddle2);
	objectBufferInit(table);

	//create shaders
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//load shader
	mvShader vs("VertexShader.txt");

	//check shader status
	if(!vs.isShaderRdy())
    {
        std::cerr << "[ERROR] VERTEXSHADER NOT LOADED: ";
		std::cerr << vs.getError() << std::endl;
        return false;
    }

	//load shader
	mvShader fs("FragShader.txt");

	//check shader status
	if(!fs.isShaderRdy())
    {
        std::cerr << "[ERROR] VERTEXSHADER NOT LOADED: ";
		std::cerr << fs.getError() << std::endl;
        return false;
    }
	
	//convert to const char for glut
	const char *vs_c_str = vs.getShaderString();
	const char *fs_c_str = fs.getShaderString();
	
    //compile the shaders
    GLint shader_status;

    // Vertex shader first
    glShaderSource(vertex_shader, 1, &vs_c_str, NULL);
    glCompileShader(vertex_shader);

    //check the compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader, 1, &fs_c_str, NULL);
    glCompileShader(fragment_shader);

    //check the compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
        return false;
    }

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program, const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_color = glGetAttribLocation(program, const_cast<const char*>("v_color"));
    if(loc_color == -1)
    {
        std::cerr << "[F] COLOR NOT FOUND" << std::endl;
        return false;
    }

    loc_norm = glGetAttribLocation(program, const_cast<const char*>("v_norm"));
    if(loc_norm == -1)
    {
        std::cerr << "[F] NORM NOT FOUND" << std::endl;
        return false;
    }

    loc_uv = glGetAttribLocation(program, const_cast<const char*>("v_uv"));
    if(loc_uv == -1)
    {
        std::cerr << "[F] UV NOT FOUND" << std::endl;
        return false;
    }

    loc_texSampler = glGetUniformLocation(program, const_cast<const char*>("texSampler"));
    if(loc_texSampler == -1)
    {
        std::cerr << "[F] TEXSAMPLER NOT FOUND" << std::endl;
        return false;
    }

    loc_ModelView = glGetUniformLocation(program, const_cast<const char*>("ModelView"));
    if(loc_ModelView == -1)
    {
        std::cerr << "[F] MODELVIEW NOT FOUND" << std::endl;
        return false;
    }

	loc_Projection = glGetUniformLocation(program, const_cast<const char*>("Projection"));
    if(loc_Projection == -1)
    {
        std::cerr << "[F] PROJECTION NOT FOUND" << std::endl;
        return false;
    }
	
	loc_lightOne = glGetUniformLocation(program, const_cast<const char*>("lightOne"));
    if(loc_lightOne == -1)
    {
        std::cerr << "[F] LIGHTONE NOT FOUND" << std::endl;
        return false;
    }
	loc_lightTwo = glGetUniformLocation(program, const_cast<const char*>("lightTwo"));
    if(loc_lightTwo == -1)
    {
        std::cerr << "[F] LIGHTTWO NOT FOUND" << std::endl;
        return false;
    }
	loc_AP = glGetUniformLocation(program, const_cast<const char*>("AP"));
    if(loc_AP == -1)
    {
        std::cerr << "[F] AP NOT FOUND" << std::endl;
        return false;
    }
	loc_DP = glGetUniformLocation(program, const_cast<const char*>("DP"));
    if(loc_DP == -1)
    {
        std::cerr << "[F] DP NOT FOUND" << std::endl;
        return false;
    }
	loc_SP = glGetUniformLocation(program, const_cast<const char*>("SP"));
    if(loc_SP == -1)
    {
        std::cerr << "[F] SP NOT FOUND" << std::endl;
        return false;
    }
	loc_shininess = glGetUniformLocation(program, const_cast<const char*>("shininess"));
    if(loc_shininess == -1)
    {
        std::cerr << "[F] SHININESS NOT FOUND" << std::endl;
        return false;
    }
    
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(0.0, sin(50.0)*30, cos(50.0)*-30.0), //Eye Position - old eye position: glm::vec3(0.0, 8.0, -16.0)
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(width)/float(height), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    return true;
}

void mvDisplay::display()
{
        
    //clear the screen
    glClearColor(0.0, 0.3, 1.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    text.printw (-0.42, -0.95, 0, "PlayerOne %i - %i PlayerTwo\0", score_p1, score_p2);

	displayObject(puck);
	displayObject(paddle1);
	displayObject(paddle2);
	displayObject(table);

    //swap the buffers
    glutSwapBuffers();
}
	
void mvDisplay::reshape(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    //Change the viewport to be correct
    glViewport( 0, 0, width, height);
    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(width)/float(height), 0.01f, 100.0f);
}
	
mvPuck* mvDisplay::getPuckReference()
{
	return &puck;
}

mvPaddle* mvDisplay::getPaddle1Reference()
{
	return &paddle1;
}

mvPaddle* mvDisplay::getPaddle2Reference()
{
	return &paddle2;
}

mvTable* mvDisplay::getTableReference()
{
	return &table;
}

void mvDisplay::toggleLightOne()
{
	if(lightOne.w==1.0)
		lightOne.w=0.0;
	else
		lightOne.w=1.0;
}

void mvDisplay::toggleLightTwo()
{
	if(lightTwo.w==1.0)
		lightTwo.w=0.0;
	else
		lightTwo.w=1.0;
}

void mvDisplay::setCamPos(glm::vec3 camPos)
{
	view = glm::lookAt(camPos,						//Eye Position
                        glm::vec3(0.0, 0.0, 0.0),	//Focus point
                        glm::vec3(0.0, 1.0, 0.0));	//Positive Y is up
}

void mvDisplay::addScore(int player)
{
	if (player == 1)
        score_p1++;
    else
        score_p2++;
}

void mvDisplay::resetScore()
{
    score_p1=0;
    score_p2=0;
}

int mvDisplay::checkForWinner ()
{
    if (score_p1 == 7)
        return 1;
    else if (score_p2 == 7)
       return 2;
    else
        return 0;
}

void mvDisplay::objectBufferInit(mvObject &object)
{
	//set object buffers
	glGenBuffers(1, &object.vbo_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo_geometry);
	glBufferData(GL_ARRAY_BUFFER, object.sizeofgeometry(), object.geometry, GL_STATIC_DRAW);
}

void mvDisplay::displayObject(mvObject &object)
{
    //premultiply the matrix for this example
    glm::mat4 mv = view * object.model;

    //enable the shader program
    glUseProgram(program);

	if(object.hasTexture())
		object.bind();

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_ModelView, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(loc_Projection, 1, GL_FALSE, glm::value_ptr(projection));
	
	glUniform4fv(loc_lightOne,1,glm::value_ptr(lightOne));
	glUniform4fv(loc_lightTwo,1,glm::value_ptr(lightTwo));
	glUniform4fv(loc_AP,1,glm::value_ptr(AP));
	glUniform4fv(loc_DP,1,glm::value_ptr(DP));
	glUniform4fv(loc_SP,1,glm::value_ptr(SP));

	glUniform1f(loc_shininess,shininess);
	
    glUniform1i(loc_texSampler, 0);

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glEnableVertexAttribArray(loc_norm);
    glEnableVertexAttribArray(loc_uv);
    glBindBuffer(GL_ARRAY_BUFFER, object.vbo_geometry);
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(mvVertex),//stride
                           (void*)offsetof(mvVertex,position));//offset
	
    glVertexAttribPointer( loc_color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(mvVertex),
                           (void*)offsetof(mvVertex,color));

    glVertexAttribPointer( loc_norm,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(mvVertex),
                           (void*)offsetof(mvVertex,normal));

    glVertexAttribPointer( loc_uv,
                           2,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(mvVertex),
                           (void*)offsetof(mvVertex,uv));

	glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
    glDisableVertexAttribArray(loc_norm);
    glDisableVertexAttribArray(loc_uv);
}