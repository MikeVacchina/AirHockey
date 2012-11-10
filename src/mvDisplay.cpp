#include "mvDisplay.h"


mvDisplay::mvDisplay()
{
	//set default values
	width = 640;
	height = 480;

	started = false;

	//load maze objects
	maze1.loadMaze("maze1.mv");
	maze2.loadMaze("maze2.mv");

	//load sphere object and translate/scale mesh
	sphere.loadMesh("sphere.obj");
	sphere.setColor(1.0,1.0,0.0);
	double r = 0.5/sphere.getMeshRadius();
	sphere.scale(r);
	r = sphere.getMeshRadius();
	sphere.translate(0.0,r,0.0);
	
	lightOne = glm::vec4(10.0,0.0,0.0,1.0);
	lightTwo = glm::vec4(-10.0,5.0,5.0,1.0);

	AP = glm::vec4(0.2,0.2,0.2,1.0);
	DP = glm::vec4(0.7,0.7,0.7,1.0);
	SP = glm::vec4(0.0,0.0,0.0,1.0);

	shininess = 100.0;

	//dragon.assimpLoadMesh("dragon.obj");

	//dragon.setColor(1.0,0.2,0.2);
	//
	//glm::mat4 dragonRot = glm::rotate(glm::mat4(1.0f), float(90.0), glm::vec3(1.0,0.0,0.0));
	//glm::mat4 dragonScale = glm::scale(glm::mat4(1.0f),glm::vec3(0.2,0.2,0.2));
	//glm::mat4 dragonTrn = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,1.75,5.0));

	//dragonModel = dragonTrn * dragonScale * dragonRot;
	//dragon.model = dragonModel;

	//texCube.loadMesh("cubeTexture.obj");

	////glm::mat4 texCubeRot = glm::rotate(glm::mat4(1.0f), glm::vec3(1.0,0.0,0.0));
	//glm::mat4 texCubeScale = glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5));
	//glm::mat4 texCubeTrn = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,1.5,-5.0));

	//texCubeModel = texCubeTrn * texCubeScale;// * texCubeRot;
	//texCube.model = texCubeModel;
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

bool mvDisplay::initializeDisplayResources()
{
	//initialize objects
	objectBufferInit(maze1);
	objectBufferInit(maze2);
	objectBufferInit(sphere);
	//objectBufferInit(dragon);
	//objectBufferInit(texCube);

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
    view = glm::lookAt( glm::vec3(0.0, 20.0, -10.0), //Eye Position - old eye position: glm::vec3(0.0, 8.0, -16.0)
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(width)/float(height), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	
    glUniform1i(loc_texSampler, 1);
    return true;
}

void mvDisplay::display()
{
    //clear the screen
    glClearColor(0.0, 0.0, 0.2f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(started)
	{
		//display objects
		if(maze==1)
			displayObject(maze1);
		else if(maze==2)
			displayObject(maze2);
		displayObject(sphere);
		//displayObject(dragon);
		//displayObject(texCube);
	}

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

void mvDisplay::playMaze(int mazeID)
{
	//set maze id
	maze = mazeID;
	
	if(maze==1)
	{
		//reset ball position and motion
		sphere.falling = false;
		sphere.acc = glm::vec3(0.0);
		sphere.vel = glm::vec3(0.0);
		sphere.pos = maze1.getBegin();
		//dragon.model = dragonModel;
		//texCube.model = texCubeModel;
	}
	else if(maze==2)
	{
		//reset ball position and motion
		sphere.falling = false;
		sphere.acc = glm::vec3(0.0);
		sphere.vel = glm::vec3(0.0);
		sphere.pos = maze2.getBegin();
		//dragon.model = dragonModel;
		//texCube.model = texCubeModel;
	}

	started = true;
}

void mvDisplay::setMazeModelMat(glm::mat4 m)
{
	//set the correct maze model matrix
	if(maze==1)
		maze1.model = m;
	else if(maze==2)
		maze2.model = m;
	//dragon.model = m*dragonModel;
	//texCube.model = m*texCubeModel;
}

void mvDisplay::setBallModelMat(glm::mat4 m)
{
	//set ball model matrix
	sphere.model = m;
}

mvMaze* mvDisplay::getMaze()
{
	if(started)
		//return the proper maze reference
		if(maze==1)
			return &maze1;
		else if(maze==2)
			return &maze2;
	else
		return NULL;
}

mvSphere* mvDisplay::getSphere()
{
	//return the sphere reference
	if(started)
		return &sphere;
	else
		return NULL;
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

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_ModelView, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(loc_Projection, 1, GL_FALSE, glm::value_ptr(projection));
	
	glUniform4fv(loc_lightOne,1,glm::value_ptr(lightOne));
	glUniform4fv(loc_lightTwo,1,glm::value_ptr(lightTwo));
	glUniform4fv(loc_AP,1,glm::value_ptr(AP));
	glUniform4fv(loc_DP,1,glm::value_ptr(DP));
	glUniform4fv(loc_SP,1,glm::value_ptr(SP));

	glUniform1f(loc_shininess,shininess);

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

	if(object.hasTexture())
		object.bind();

	glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
    glDisableVertexAttribArray(loc_norm);
    glDisableVertexAttribArray(loc_uv);
}