#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream> 

#include <fstream>

#include "Graph.h"

#include <list>

#include <utility> // for STL pair

#include <string>

#include <map>

using std::pair;

using namespace std;


//typedef GraphArc<char, int> Arc;
//typedef GraphNode<char, int> Node;

typedef GraphArc<std::pair<string, int>, int> Arc1;
typedef GraphNode<std::pair<string, int>, int> Node1;

void visit( Node1 * pNode ) {
	cout << "Visiting: " << pNode->data().second << endl;
}

int main(int argc, char *argv[]) {
   
	//Q1

	//Graph<int, int> graph(25);

	sf::Texture nodeTex;
	sf::Texture nodeSelectedTex;
	sf::Texture nodeHighlighted;
	sf::Texture nodeVisisted;

	sf::Texture startTex;
	sf::Texture resetTex;

	startTex.loadFromFile("start.png");
	resetTex.loadFromFile("reset.png");

	sf::Font font;
	font.loadFromFile("arial.ttf");

	sf::Sprite startSprite;
	sf::Sprite resetSprite;

	startSprite.setTexture(startTex);
	resetSprite.setTexture(resetTex);

	startSprite.setPosition(sf::Vector2f(1050, 600));

	resetSprite.setPosition(sf::Vector2f(1050, 700));


	bool startSelected = false;
	bool destSelected = false;

	bool onlyOnce = false;

	bool startClicked = false;

	int startNum = 0;
	int endNum = 0;

	nodeTex.loadFromFile("Node.png");
	nodeSelectedTex.loadFromFile("SelectedNode.png");
	nodeHighlighted.loadFromFile("HighlightedNode.png");
	nodeVisisted.loadFromFile("VisistedNode.png");

	Graph< pair<string, int>, int > myGraph(30);

	char i1[6];
	int max = INT_MAX;
	int i2 = 0;
	ifstream myfile;
	std::vector<Node1*> path;
	myfile.open("Nodes.txt");

	std::map<std::string, std::vector<std::pair<std::string, int>>> nodeMap;

	//nodeMap["Aldor"] = 

	while( myfile >> i1 )
	{
		myGraph.addNode(std::pair<std::string, int>(i1, max), i2++);
	}

	myfile.close();

	myfile.open("NodePositions.txt");

	int node, x, y;

	while ( myfile >> node >> x >> y )
	{
		myGraph.addPos(node, x, y);
	}

	myfile.close();

	myfile.open("Arcs.txt");

	int from, to, weight;

	while ( myfile >> from >> to >> weight )
	{
		myGraph.addArc(from, to, weight);
	}

	myfile.close();

	for(int i = 0; i < myGraph.size(); i++)
	{
		myGraph.nodeArray()[i]->setCost(0);
	}

	//myGraph.aStar(myGraph.nodeArray()[0], myGraph.nodeArray()[29], visit, path);

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1200, 800, 32), "A*");

	// Start game loop
	while (window.isOpen())
	{
		window.clear();

		sf::Event event;

		while(window.pollEvent(event))
		{
			float MouseX = sf::Mouse::getPosition(window).x;
			float MouseY = sf::Mouse::getPosition(window).y;

			for(int i = 0; i < myGraph.size(); i++)
			{
				if((MouseX < myGraph.nodeArray()[i]->getX() + 20 && MouseX > myGraph.nodeArray()[i]->getX() - 20) && (MouseY < myGraph.nodeArray()[i]->getY() + 20 && MouseY > myGraph.nodeArray()[i]->getY() - 20))
				{
					myGraph.nodeArray()[i]->setHighlighted(true);
				}
				else
				{
					myGraph.nodeArray()[i]->setHighlighted(false);
				}
			}

			if(event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				float MouseX = sf::Mouse::getPosition(window).x;
				float MouseY = sf::Mouse::getPosition(window).y;

				if(onlyOnce == false)
				{
					if(startSelected == false)
					{
						for(int i = 0; i < myGraph.size(); i++)
						{
							if((MouseX < myGraph.nodeArray()[i]->getX() + 20 && MouseX > myGraph.nodeArray()[i]->getX() - 20) && (MouseY < myGraph.nodeArray()[i]->getY() + 20 && MouseY > myGraph.nodeArray()[i]->getY() - 20))
							{
								startSelected = true;
								startNum = i;
								myGraph.nodeArray()[i]->setRemoved(true);
							}
						}
					}
					else if(startSelected == true && destSelected == false)
					{
						for(int i = 0; i < myGraph.size(); i++)
						{
							if((MouseX < myGraph.nodeArray()[i]->getX() + 20 && MouseX > myGraph.nodeArray()[i]->getX() - 20) && (MouseY < myGraph.nodeArray()[i]->getY() + 20 && MouseY > myGraph.nodeArray()[i]->getY() - 20))
							{
								destSelected = true;
								endNum = i;
								myGraph.nodeArray()[i]->setRemoved(true);
							}
						}

					}

				}
				if(startClicked == false && startSelected == true && destSelected == true)
				{
					if((MouseX < startSprite.getPosition().x + 80 && MouseX > startSprite.getPosition().x) && (MouseY < startSprite.getPosition().y + 50 && MouseY > startSprite.getPosition().y))
					{
						startClicked = true;
					}
				}

				if((MouseX < resetSprite.getPosition().x + 80 && MouseX > resetSprite.getPosition().x) && (MouseY < resetSprite.getPosition().y + 50 && MouseY > resetSprite.getPosition().y))
				{
					for(int r = 0; r < myGraph.size(); r++)
					{
						myGraph.nodeArray()[r]->setPrevious(NULL);
						myGraph.nodeArray()[r]->setMarked(false);
						myGraph.nodeArray()[r]->data().second = INT_MAX;
						myGraph.nodeArray()[r]->setHueristic(0);
						myGraph.nodeArray()[r]->setRemoved(false);

					}

					onlyOnce = false;
					startSelected = false;
					destSelected = false;
					startClicked = false;
				}

			}
		}

		if((startSelected == true && destSelected == true) && onlyOnce == false && startClicked == true)
		{
			myGraph.aStar(myGraph.nodeArray()[startNum], myGraph.nodeArray()[endNum], visit, path);
			onlyOnce = true;
		}

		for(int i = 0; i < myGraph.size(); i++)
		{
			list<Arc1>::const_iterator iter = myGraph.nodeArray()[i]->arcList().begin();
			list<Arc1>::const_iterator endIter = myGraph.nodeArray()[i]->arcList().end();
        
			std::vector<sf::Vector2f> positions;
			std::vector<sf::Text> texts;

			for( ; iter != endIter; iter++)
			{
				sf::Vector2f tempVec;
				tempVec.x = (*iter).node()->getX();
				tempVec.y = (*iter).node()->getY();
				positions.push_back(tempVec);

				sf::Text text;

				text.setFont(font);
				text.setString(to_string((*iter).weight()));
				text.setCharacterSize(20);
				text.setColor(sf::Color::Red);
				tempVec.x = (tempVec.x + myGraph.nodeArray()[i]->getX()) / 2;
				tempVec.y = (tempVec.y + myGraph.nodeArray()[i]->getY()) / 2;
				text.setPosition(tempVec);

				texts.push_back(text);
			}

			for(int j = 0; j < positions.size(); j++)
			{
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(myGraph.nodeArray()[i]->getX(), myGraph.nodeArray()[i]->getY())),
					sf::Vertex(positions[j])
				};
				window.draw(line, 2, sf::Lines);
			}

			for(int j = 0; j < texts.size(); j++)
			{
				window.draw(texts[j]);
			}

		}

		for(int i = 0; i < myGraph.size(); i++)
		{
			sf::Sprite sprite;
			if(myGraph.nodeArray()[i]->getHighlighted() == true)
			{
				sprite.setTexture(nodeHighlighted);
			}
			else if(myGraph.nodeArray()[i]->getRemoved() == true)
			{
				sprite.setTexture(nodeSelectedTex);
			}
			else if(myGraph.nodeArray()[i]->data().second != 2147483647)
			{
				sprite.setTexture(nodeVisisted);
			}
			else
			{
				sprite.setTexture(nodeTex);
			}

			sprite.setPosition(myGraph.nodeArray()[i]->getX() - 20, myGraph.nodeArray()[i]->getY() - 20);

			window.draw(sprite);

			sf::Text text;
			text.setFont(font);
			text.setString(myGraph.nodeArray()[i]->data().first);
			text.setCharacterSize(20);
			text.setColor(sf::Color::Black);
			text.setPosition(sf::Vector2f(myGraph.nodeArray()[i]->getX() - 5, myGraph.nodeArray()[i]->getY() - 10));

			window.draw(text);

			if(myGraph.nodeArray()[i]->data().second == 2147483647)
			{
				text.setString("G[n]: Not Visited");
			}
			else
			{
				text.setString("G[n]: " + to_string(myGraph.nodeArray()[i]->data().second));
			}
			text.setCharacterSize(10);
			text.setColor(sf::Color::Cyan);
			text.setPosition(sf::Vector2f(myGraph.nodeArray()[i]->getX() - 25, myGraph.nodeArray()[i]->getY() - 35));

			window.draw(text);

			text.setString(" H[n]: " + to_string((int)myGraph.nodeArray()[i]->getHueristic()));
			text.setCharacterSize(10);
			text.setColor(sf::Color::Magenta);
			text.setPosition(sf::Vector2f(myGraph.nodeArray()[i]->getX() - 30, myGraph.nodeArray()[i]->getY() + 25));

			window.draw(text);

			//text.setString(" G[n]: " + to_string((int)myGraph.nodeArray()[i]->getCost()));
			//text.setCharacterSize(10);
			//text.setColor(sf::Color::Yellow);
			//text.setPosition(sf::Vector2f(myGraph.nodeArray()[i]->getX() - 30, myGraph.nodeArray()[i]->getY() + 40));

			//window.draw(text);
		}

		window.draw(startSprite);
		window.draw(resetSprite);


		window.display();
	}

	system("PAUSE");
}

class NodeSearchCostComparer {
public:
	bool operator()(Node1 * n1, Node1 * n2) {
		pair<string, int> p1 = n1->data();
		pair<string, int> p2 = n2->data();
		return p1.second > p2.second; 
	}
};

