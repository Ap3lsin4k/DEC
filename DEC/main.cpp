// Designer of Electrical Circuits
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include<Windows.h>
#include <conio.h>
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma warning(disable : 4996)// to ignore "setColor deprecated"

using namespace sf;

//global variables
const int H = 11;
const int W = 15;
const int size = 45;
const int sizeSprite = 10;
const int sizeBuf = H * W * 10;
short int ctrlZY = 0;
short int limitUndo = 0;

short int countMouseElementPos = -1;
short int countMouseElementRot = 0;
Texture texLampu, texRezystor, texProv, texProvPov, texBattery, texProv3, texProv4, texDiod, texTranz, texConden, texSetka, Tback;
std::string helpString;
Font font;
Text text("", font, 45);

std::ifstream fin;
std::ofstream fout;


short int setka[sizeBuf][H][W];
short int rz[sizeBuf][H][W];
String TextBox[sizeBuf][H][W];


//prototype of functions
void undoRedo();
void mousePressedLeft(Sprite &s, int x, int y, int count0_6);
void mouseRightClick(int x, int y);
void bufferDraw(Sprite &s, int count0_6);

std::string pathToProject;

//console functions
int rkil; std::string skil[1000];
void readsave()
{
	//TODO things without save.fsd, because there is a bug
	std::ifstream sin("save.fsd");
	sin >> rkil;
	for (int i = 0; i < rkil; i++)
		sin >> skil[i];
	sin.close();
}
void writesave()
{
	std::ofstream sout("save.fsd");
	sout << rkil << std::endl;;
	for (int i = 0; i < rkil; i++)
		sout << skil[i] << std::endl;
	sout.close();
}

void showProjects()
{
	//	std::ofstream sout("save.fsd");
	std::cout << "\nNumber of projects: " << rkil << std::endl;
	for (int i = 0; i < rkil; i++)
		std::cout << "\t" << skil[i] << std::endl;
	//	sout.close();
	std::cout << "\n";
}

void mainConsoleMenu()
{
	//TODO more space in console between rows
	setlocale(LC_ALL, "");
	std::cout << "Type /help or path to a project" << std::endl;
	std::string s;
	while (true)
	{
		bool t = true;
		std::cin >> s;
		if (s[0] != '/')
		{
			//	std::cout << "Type name of the file\n";
			pathToProject = s;
			//std::cin >> pathToProject;
			//std::getline(std::cin, pathToProject);
			if (pathToProject.length() < 5 || !(pathToProject[pathToProject.length() - 5] == '.' && pathToProject[pathToProject.length() - 4] == 'v' && pathToProject[pathToProject.length() - 3] == 'k' && pathToProject[pathToProject.length() - 2] == 'e' && pathToProject[pathToProject.length() - 1] == 's'))
				pathToProject += ".dec";
			for (int i = 0; i < rkil; i++)
				if (skil[i] == pathToProject) { t = false; break; }
			if (t) { skil[rkil] = pathToProject; rkil++; }
			writesave();
			return;
		}//TODO switchcase
		else if (s == "/clr")
		{
			char a[1000][500];
			for (int i = 0; i < rkil; i++)
			{
				a[i][0] = 's'; a[i][1] = 'a'; a[i][2] = 'v'; a[i][3] = 'e'; a[i][4] = '/';
				for (int j = 0; j < skil[i].length(); j++)
				{
					a[i][j + 5] = skil[i][j];
				}
				remove(a[i]);
			}
			rkil = 0;
			writesave();
			std::cout << "Projects have been deleted" << std::endl; // TODO show number of projects
		}
		else if (s == "/show")
		{
			showProjects();
		}
		else if (s == "/help")
		{
			std::cout << "/clr - delete all the projects" << std::endl;
			std::cout << "/show - to see all the projects in the directory" << std::endl;
		}
		else {
			std::cout << "Error: incorrect command." << std::endl;
			std::cout << "Please, enter:" << std::endl;
			std::cout << "-name of the existed project" << std::endl;
			std::cout << "-new name to create project and start working" << std::endl;
			std::cout << "-or type /help to see all the commands"  << std::endl;  // TODO change color of /help

		}
	}
	system("pause");
}

int main()
{
	//initialization
	Vector2i mouse;
	char symbol;
	bool isEventMouseLeftClick = false;

	// texture loading
	texLampu.loadFromFile("images/lampa1.png");
	texRezystor.loadFromFile("images/rezustor.png");
	texProv.loadFromFile("images/prov.png");
	texProvPov.loadFromFile("images/provpov.png");
	texBattery.loadFromFile("images/battery.png");
	texProv3.loadFromFile("images/prov3.png");
	texProv4.loadFromFile("images/prov4.png");
	texDiod.loadFromFile("images/key.png");
	texTranz.loadFromFile("images/tranzustor3.png");
	texConden.loadFromFile("images/condensator.png");


	Tback.loadFromFile("images/5.png");
	//todo setka-> grid
	texSetka.loadFromFile("images/setka.png");
	Sprite spriteSetka;
	spriteSetka.setTexture(texSetka);
	spriteSetka.setScale(3, 3);
	spriteSetka.setPosition(0, 0);
	spriteSetka.setColor(Color(255, 255, 255, 100));

	Sprite element[sizeSprite];
	element[0].setTexture(texProv);
	element[1].setTexture(texProvPov);
	element[2].setTexture(texProv3);
	element[3].setTexture(texProv4);
	element[4].setTexture(texRezystor);
	element[5].setTexture(texLampu);
	element[6].setTexture(texBattery);
	element[7].setTexture(texDiod);
	element[8].setTexture(texTranz);
	element[9].setTexture(texConden);

	Sprite back;
	back.setTexture(Tback);
	back.setPosition(0, 0);

	font.loadFromFile("font/10700.ttf");
	text.setColor(Color::Black);//TODO change to setFillColor


	//load icon
	Image icon;
	icon.loadFromFile("images/lampa1.png");
	//	return EXIT_FAILURE;

	readsave();
	mainConsoleMenu();  // there is waiting loop AKA infinity loop AKA while(true) until you type in the right way

	//hide the console window
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	///TODO important: adapted to the screen 
	RenderWindow window(sf::VideoMode(1000, 600), "DEC [" + pathToProject + "]", sf::Style::Close);// TODO version

	/*
	GetSystemMetrics(SM_CXSCREEN) - width of user's screen
	GetSystemMetrics(SM_CYSCREEN) - height of user's screen
	*////coordinates to draw the window at the center.
	int scrX = GetSystemMetrics(SM_CXSCREEN) / 2 - window.getSize().x / 2;
	int scrY = GetSystemMetrics(SM_CYSCREEN) / 2 - window.getSize().y / 2;
	

	window.setPosition(Vector2i(scrX, scrY));
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());




	//TODO rename save to projects
	fin.open("save/" + pathToProject);

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (fin.is_open())
			{
				fin >> setka[0][i][j];
				fin >> rz[0][i][j];
				getline(fin, helpString);

				TextBox[0][i][j] = "";
				for (int ij = 1; ij < helpString.size(); ij++) //переписати все починаючи з другого знака (перший знак пропускаємо так як це пробіл)
				{
					TextBox[0][i][j] += helpString[ij];
				}
			}
			else
			{
				setka[0][i][j] = 0;
				rz[0][i][j] = 0;
				TextBox[0][i][j] = "";
			}
		}

	}
	for (int countMas = 1; countMas < sizeBuf; countMas++) // 10 its size all bufer
		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				setka[countMas][i][j] = 0;
				rz[countMas][i][j] = 0;
				TextBox[countMas][i][j] = "";
			}

		}

	fin.close();


	while (window.isOpen())
	{
		//position of the mouse relative to the window
		mouse = Mouse::getPosition(window);

		//event
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					isEventMouseLeftClick = true;
				}
				if (event.mouseButton.button == Mouse::Middle)
				{
					//	if (!(mouse.x <= W * size && mouse.y <= H * size && mouse.x >= 0 && mouse.y >= 0))
					//	{
					//		countMouseElementPos = -1;
					//		countMouseElementRot = 0;

					//	}
					//	else //if (setka[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] != 0)
					{
						countMouseElementPos = setka[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] - 1;
						countMouseElementRot = rz[ctrlZY][int(mouse.y / size)][int(mouse.x / size)];
					}

				}
			}
			if (event.type == Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					isEventMouseLeftClick = false;
				}
			}


			if (event.type == Event::KeyPressed)
			{
				//rotation
				if (event.key.code == Keyboard::Tab)
				{
					//	if (mouse.x <= W * size && mouse.y <= H * size)
					{
						if (setka[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] == 0 || !(mouse.x <= W * size) || !(mouse.y <= H * size))
						{
							countMouseElementRot++;

							if (countMouseElementRot >= 4)
								countMouseElementRot -= 4;
						}
						else if (setka[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] != 0)
						{
							undoRedo();

							rz[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] ++;

							if (rz[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] >= 4)
								rz[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] -= 4;
						}
					}
				}
				// undo / redo
				if (event.key.control) {// TODO faster hotkeys without Ctrl

					if (event.key.code == Keyboard::Num1)
					{
						countMouseElementPos = 0;
					}
					else if (event.key.code == Keyboard::Num2)
					{
						countMouseElementPos = 1;
					}
					else if (event.key.code == Keyboard::Num3)
					{
						countMouseElementPos = 2;
					}
					else if (event.key.code == Keyboard::Num4)
					{
						countMouseElementPos = 3;
					}
					else if (event.key.code == Keyboard::Num5)
					{
						countMouseElementPos = 4;
					}
					else if (event.key.code == Keyboard::Num6)
					{
						countMouseElementPos = 5;
					}
					else if (event.key.code == Keyboard::Num7)
					{
						countMouseElementPos = 6;
					}
					else if (event.key.code == Keyboard::Num8)
					{
						countMouseElementPos = 7;
					}
					else if (event.key.code == Keyboard::Num9)
					{
						countMouseElementPos = 8;
					}
					else if (event.key.code == Keyboard::Num0)
					{
						countMouseElementPos = 9;
					}



					else if (event.key.code == Keyboard::Z)
					{
						if (ctrlZY < sizeBuf - 1 && ctrlZY < limitUndo)// index elementiv pochunaetsya z 0 i zakinchuyetsya 2 (if Buf = 3)
						{
							ctrlZY++;
						}
					}

					else if (event.key.code == Keyboard::X)
					{
						if (ctrlZY > 0)
						{
							ctrlZY--;
						}

					}

					else if (event.key.code == Keyboard::S)
					{
						fout.open("save/" + pathToProject);
						for (int i = 0; i < H; i++)
						{
							for (int j = 0; j < W; j++)
							{
								fout << setka[ctrlZY][i][j] << ' ';
								fout << rz[ctrlZY][i][j] << ' ';
								helpString = TextBox[ctrlZY][i][j];
								fout << helpString;

								fout << "\n";
							}
							fout << std::endl;
						}


						fout.close();
					}
				}

				if (event.key.code == Keyboard::BackSpace)
				{
					if (TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)].getSize() != 0)
					{
						helpString = "";
						for (int i = 0; i < TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)].getSize() - 1; i++)
						{
							helpString += TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)][i];
						}
						TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] = helpString;
					}
				}
			}

			if (event.type == sf::Event::TextEntered)
			{
				if (setka[ctrlZY][int(mouse.y / size)][(mouse.x / size)] != 0)
				{

					//							isDigit												is V-86|		Space down'_' - 95|						   m-109|						O-79|						A-65				
					if ((event.text.unicode >= 48 && event.text.unicode < 58) || event.text.unicode == 86 || event.text.unicode == 95 || event.text.unicode == 109 || event.text.unicode == 79 || event.text.unicode == 65)
					{
						symbol = static_cast<char>(event.text.unicode);

						if (TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)].getSize() < 16)
							TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)] += symbol;
					}

				}
			}
		}

		//		window.setSize(sf::Vector2u(1000, 600));//fitcha 
		window.setPosition(Vector2i(0, 0));


		if (mouse.x >= 0 && mouse.y >= 0 && mouse.x <= 1000 && mouse.y <= 600)
		{
			text.setString(TextBox[ctrlZY][int(mouse.y / size)][int(mouse.x / size)]);
		}
		else
			text.setString("");


		//draw
		window.clear(Color(171, 171, 171, 0));

		window.draw(back);
		window.draw(spriteSetka);


		for (int indElement = 0; indElement < sizeSprite; indElement++)
		{
			bufferDraw(element[indElement], indElement);
			window.draw(element[indElement]);
		}

		//mouse
		if (isEventMouseLeftClick)
		{
			for (int i = 0; i < sizeSprite; i++)
			{
				mousePressedLeft(element[i], mouse.x, mouse.y, i);
			}
		}


		if (Mouse::isButtonPressed(Mouse::Right))
		{
			mouseRightClick(mouse.x, mouse.y);
		}

		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				for (int indElement = 0; indElement < sizeSprite; indElement++)
				{
					if (setka[ctrlZY][i][j] == indElement + 1)
					{
						element[indElement].setRotation(rz[ctrlZY][i][j] * 90);
						if (rz[ctrlZY][i][j] == 1) element[indElement].setPosition(j*size + 45, i*size);
						else if (rz[ctrlZY][i][j] == 2) element[indElement].setPosition(j*size + 45, i*size + 45);
						else if (rz[ctrlZY][i][j] == 3) element[indElement].setPosition(j*size, i*size + 45);
						else element[indElement].setPosition(j*size, i*size);
						window.draw(element[indElement]);
						continue;///deBug
					}
				}
			}
		}

		//draw elementMouse
		for (int indElement = 0; indElement < sizeSprite; indElement++)
		{
			//if (isMove[i])				
			if (countMouseElementPos == indElement)
			{
				element[indElement].setColor(Color(255, 255, 255, 200));
				element[indElement].setPosition(mouse.x, mouse.y);
				element[indElement].setRotation(countMouseElementRot * 90);

				if (countMouseElementRot == 1) element[indElement].setPosition(element[indElement].getPosition().x + size, element[indElement].getPosition().y);
				else if (countMouseElementRot == 2) element[indElement].setPosition(element[indElement].getPosition().x + size, element[indElement].getPosition().y + size);
				else if (countMouseElementRot == 3) element[indElement].setPosition(element[indElement].getPosition().x, element[indElement].getPosition().y + size);

				window.draw(element[indElement]);
				element[indElement].setColor(Color::White);
			}
		}

		text.setPosition(W*size, 0);
		window.draw(text);
		window.display();

	}

	//	std::cin >> pathToProject;
	//save the project at the end

	fout.open("save/" + pathToProject);
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			fout << setka[ctrlZY][i][j] << ' ';
			fout << rz[ctrlZY][i][j] << ' ';
			helpString = TextBox[ctrlZY][i][j];
			fout << helpString;

			fout << "\n";
		}
		fout << std::endl;
	}


	fout.close();

	return 0;

}

//functions
void undoRedo()
{
	//new cod
	if (ctrlZY != 0)
	{
		int countMas;
		for (countMas = ctrlZY; countMas < sizeBuf; countMas++)
		{
			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					setka[countMas - ctrlZY][i][j] = setka[countMas][i][j];	//[2-2]=[2]; [3-2]=[3]; [4-2]=[4]; ...
					rz[countMas - ctrlZY][i][j] = rz[countMas][i][j];		//[0] = [2]; [1] = [3]; [2] = [4]; ...
					TextBox[countMas - ctrlZY][i][j] = TextBox[countMas][i][j];
				}
			}
		}//countMas == 4 
		for (int countMas2 = countMas - ctrlZY; countMas2 < (sizeBuf - 1); countMas2++)		//[4] = 0;		[5] = 0;		[6] = 0; ...
		{
			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					setka[countMas2][i][j] = 0;								//obnulayem reshtu masuva
					rz[countMas2][i][j] = 0;
					TextBox[countMas2][i][j] = "";
				}
			}
		}
		limitUndo -= ctrlZY;
		ctrlZY = 0;

	}

	if (ctrlZY == 0)
	{
		for (int countMas = sizeBuf - 1; countMas > 0; countMas--)
			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					setka[countMas][i][j] = setka[countMas - 1][i][j];
					rz[countMas][i][j] = rz[countMas - 1][i][j];
					TextBox[countMas][i][j] = TextBox[countMas - 1][i][j];
				}
			}
		limitUndo++;
	}

}

void mousePressedLeft(Sprite &s, int x, int y, int count0_6)
{

	if (s.getGlobalBounds().contains(x, y))
	{
		countMouseElementPos = count0_6;
		countMouseElementRot = 0;
	}

	if (countMouseElementPos == count0_6)
	{

		if (x > 0 && x < W*size && y>0 && y < H*size)
		{
			if (x <= W * size && y <= H * size)
			{
				if (setka[ctrlZY][int(y / size)][int(x / size)] == 0)//якщо там де ми хочемо постaвити елeмент пусто
				{
					undoRedo();

					setka[ctrlZY][int(y / size)][int(x / size)] = countMouseElementPos + 1;
					rz[ctrlZY][int(y / size)][int(x / size)] = countMouseElementRot;
				}
			}
		}


	}

}

void mouseRightClick(int x, int y)
{
	if (x <= W * size && y <= H * size && x >= 0 && y >= 0)
	{
		if (setka[ctrlZY][int(y / size)][int(x / size)] != 0)//якщо там де ми видаляємо елементи не пусто, то
		{
			undoRedo();//даєм місце щоб записати нові дані
		}

		setka[ctrlZY][int(y / size)][int(x / size)] = 0;		//обнуляєм 
		rz[ctrlZY][int(y / size)][int(x / size)] = 0;			//обнуляєм елементи 
		TextBox[ctrlZY][int(y / size)][int(x / size)] = "";		//поза умовою (setka[][][] != 0)
	}

}

void bufferDraw(Sprite &s, int count0_6)
{
	s.setPosition(20 + 65 * count0_6, 525);
	s.setRotation(0);
}