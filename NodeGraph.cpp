#include "NodeGraph.h"

NodeGraph::NodeGraph(){
}

NodeGraph::~NodeGraph(){
}

void NodeGraph::init(){
	size = 150;
	

	for (int i = -(size/2), row = size/2; i < row; ++i){
		std::vector<CollisionNode*> new_row;
		graph.push_back(new_row);

		//initialize values for each node
		for(int j = -(size/2), col = size/2; j < col; ++j){
			CollisionNode* new_node = new CollisionNode;
			new_node->row = i + (size/2);
			new_node->column = j + (size/2);
			new_node->position = glm::vec3(2*i,2*j,-90);
			new_node->visited = false;
			new_node->fromstart = 0;
			new_node->starttogoal = 0;
			new_node->hasParent = 0;
			graph[i+size/2].push_back(new_node);
		}
	}

	for(int i = 0; i < size; ++i){
		for(int j = 0; j< size; ++j){
			//add all neighbours if in middle area
			if((i-1 > 0) && (j-1 > 0) &&(i+1 < size)&&(j+1 < size)){
				graph[i][j]->neighbours.push_back(graph[i-1][j-1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
				graph[i][j]->neighbours.push_back(graph[i-1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j-1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
				graph[i][j]->neighbours.push_back(graph[i+1][j+1]);
			} 
			//add neighbours if in corner
			//bottom left
			else if((i-1 < 0) && (j-1 < 0)){
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
			}
			//top left
			else if((i-1 < 0) && (j+1 == size)){
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j-1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
			} 
			//top right
			else if((i+1 == size) && (j+1 == size)){
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j-1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
			} 
			//bottom right
			else if((i+1 == size) && (j-1 < 0)){
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
			}
			//add neighbours if against wall
			//left wall
			else if (i-1 < 0){
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
				graph[i][j]->neighbours.push_back(graph[i+1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j-1]);
			}
			//right wall
			else if (i+1 == size){
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
				graph[i][j]->neighbours.push_back(graph[i-1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i-1][j-1]);
			}
			//floor
			else if (j-1 < 0){
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
				graph[i][j]->neighbours.push_back(graph[i-1][j+1]);
				graph[i][j]->neighbours.push_back(graph[i][j+1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j+1]);
			}
			//ceiling
			else if (j+1 == size){
				graph[i][j]->neighbours.push_back(graph[i-1][j]);
				graph[i][j]->neighbours.push_back(graph[i+1][j]);
				graph[i][j]->neighbours.push_back(graph[i-1][j-1]);
				graph[i][j]->neighbours.push_back(graph[i][j-1]);
				graph[i][j]->neighbours.push_back(graph[i+1][j-1]);
			}

		}
	}
}

void NodeGraph::draw(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 0.0f, 0.0f);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	


	for(int i = 0, row = size; i < row; ++i){
		for(int j = 0, col = size; j < col; ++j){
			if(graph[i][j]->visited)
				glColor3f(0,1,0);
			else
				glColor3f(0,0,0);
			glm::vec3 temp_pos = graph[i][j]->position;
			glBegin(GL_QUADS);
			glVertex3f(temp_pos.x-1,temp_pos.y+1,temp_pos.z);
			glVertex3f(temp_pos.x+1,temp_pos.y+1,temp_pos.z);
			glVertex3f(temp_pos.x+1,temp_pos.y-1,temp_pos.z);
			glVertex3f(temp_pos.x-1,temp_pos.y-1,temp_pos.z);
			glEnd();
		}
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//glPopMatrix();

	//Reset colour
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
}

void NodeGraph::reset(){
	for(int i = 0; i < size; ++i){
		for(int j = 0; j<size;++j){
			graph[i][j]->camefrom = NULL;
			graph[i][j]->fromstart = 0;
			graph[i][j]->starttogoal = 0;
			graph[i][j]->hasParent = 0;
			graph[i][j]->visited = 0;
		}
	}
}