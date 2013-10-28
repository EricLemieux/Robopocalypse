#include "Engine.h"
// TODO: 
//Frenet Frame for orientation calculations(compute, dir, right, up then merge for matrix) right = dir cross y, up = right cross dir, Rmat = [r,u,d]
//Framework the shit out of OBJModel so that it accepts textures/binds per obj instead of doing that in main
//add multiple obj support
//camera movement, centered between two chars, zooms to keep them in frame. Optional, rotate camera during sidestep to new orientation.
//obj movement/physics

GLfloat yspeed = 0.0f;
GLfloat zstep = 0.0f;
GLfloat xspeed = 0.0f;
GLfloat xstep = 0.0f;
GLfloat x2step = 0.0f;
GLfloat z2step = 0.0f;
GLfloat camDist = -5.0f;

void drawFunc(Assets assetList);

int main()
{
	//window has constructor with same param as .create
	sf::RenderWindow window;
	window.create(sf::VideoMode(1400,800),"WINDOOOOW");
	window.setFramerateLimit(30);

	Assets assetList;
	assetList.LoadAssets("assets.txt");

	//add object to player
	Player player1(assetList.objects[0]);
	Player player2(assetList.objects[1]);

	//Camera stuff
	Camera camera;

	glClearDepth(1.f);
	glClearColor(0.5f, 0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINEAR_MIPMAP_LINEAR);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, float(window.getSize().x)/float(window.getSize().y),2.f,2000.f);
	//gluLookAt(0,0,0,0,0,-10,0,1,0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	bool running = true;
	
	while(running){
		sf::Event event;

		//update loop
		while(window.pollEvent(event)){

			//close window if event returns closed
			if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,6)){
				running = false;
			} else if (event.type == sf::Event::Resized){
				glViewport(0,0,event.size.width, event.size.height);
			}
		}

		//keyboard input
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			player2.decreaseVelX(0.2f);
			//yspeed += 10.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			player2.increaseVelX(0.2f);
			//yspeed -= 10.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			xspeed += 10.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			xspeed -= 10.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			zstep += 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			zstep -= 1.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			player1.increaseVelX(0.2f);
			//xstep += 0.5f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			player1.decreaseVelX(0.2f);
			//xstep -= 0.5f;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			z2step += 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			z2step -= 1.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			x2step += 0.5f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			x2step -= 0.5f;
		*/
		

		camera.setFocus(player1, player2);
		camera.update();

		//clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisableClientState(GL_COLOR_ARRAY);


		//controller input
		checkLeftJoystick(0, player1);
        checkLeftJoystick(1, player2); 

		//Update the bounding boxes with the new positions.
		//TODO make seperate function
		assetList.boundingBoxes[0].setPos(player1.getPos());
		assetList.boundingBoxes[1].setPos(player2.getPos());

		player1.update(assetList.boundingBoxes, 0);
		player2.update(assetList.boundingBoxes, 1);

		player1.draw();
		player2.draw();
		drawFunc(assetList);

		glDisable(GL_TEXTURE_2D);
		//end frame
		window.display();

		
		
	}
	return 0;
}

void drawFunc(Assets assetList){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.f,0.f,camDist);
//	glColor3f(0.f,0.f,1.f);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glGenTextures;
	sf::Vector3f tempVertex;
	sf::Vector2f tempUV;
	sf::Vector3f tempNorm;
	int a = assetList.objects[0].getVerSize();
	int b = assetList.objects[0].getUVSize();
	int c = assetList.objects[0].getNormSize();


	glPushMatrix();

	glTranslatef(xstep, 0.f, assetList.objects[2].getPosZ());

	glRotatef(yspeed, 0.0f, 1.f, 0.f);
	glRotatef(xspeed, 1.0f, 0.0f, 0.f);

	glBegin(GL_TRIANGLES);

	
	for(int j = 0; j < assetList.objects[2].getVerSize(); j++){
		tempVertex = assetList.objects[2].getVertex(j);
		tempUV = assetList.objects[2].getUV(j);
		tempNorm = assetList.objects[2].getNormal(j);
		
		glNormal3f( tempNorm.x,tempNorm.y,tempNorm.z);
		
		glTexCoord2f(tempUV.x,tempUV.y);
		
		glVertex3f( tempVertex.x,tempVertex.y,tempVertex.z);
	}
	
	glEnd();
	glPopMatrix();
}