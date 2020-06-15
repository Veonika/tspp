//#include <map>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "level.h"
using namespace sf;
using namespace std;

View view;
RenderWindow window(VideoMode(620, 460), "GAME");

enum states
{
	STATE_LEFT,
	STATE_RIGHT,
	STATE_UP,
	STATE_DOWN
};

class Entity 
{
	float dx, dy, x, y, speed, moveTimer;//усокрения, координаты, скорость и таймер
	float w, h; //ширина и высота спрайта
	bool life, isMove;
	int n_rect;
	states state; //добавляем тип перечисления - состояние объекта
	Texture texture;
	String name; //враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	public:
		Sprite sprite;
		vector<Object> obj;//вектор объектов карты
		Entity(Image &image, float X, float Y, float W, float H, string _name) 
		{
			x = X; y = Y; w = W; h = H; moveTimer = 0; n_rect = 1;
			speed = 0; dx = 0; dy = 0; //либо для каждого класса прописать отдельно
			life = true; isMove = false; state = STATE_RIGHT; name = _name;
			texture.loadFromImage(image); //загружаем название файла изображения
			sprite.setTexture(texture);
			sprite.setOrigin(w / 2, h / 2);
		}
		float getx()
		{
			return x;
		}
		void setx(float _x)
		{
			x = _x;
		}
		float gety()
		{
			return y;
		}
		void sety(float _y)
		{
			y = _y;
		}
		float getw()
		{
			return w;
		}
		void setw(float _w)
		{
			w = _w;
		}
		float geth()
		{
			return h;
		}
		void seth(float _h)
		{
			h = _h;
		}
		int getn_rect()
		{
			return n_rect;
		}
		void setn_rect(int _n_rect)
		{
			n_rect = _n_rect;
		}
		bool getlife()
		{
			return life;
		}
		void setlife(bool _life)
		{
			life = _life;
		}
		/*void setsprite(IntRect rectangle)
		{
			sprite.setTextureRect(rectangle);
		}
		void setspriteposition(int _w, int _h)
		{
			sprite.setPosition(_w,_h);
		}*/
		int getstate()
		{
			return state;
		}
		void setstate(int _state)
		{
			state = static_cast<states>(_state);
		}
		float getspeed()
		{
			return speed;
		}
		void setspeed(float _speed)
		{
			speed = _speed;
		}
		float getdx()
		{
			return dx;
		}
		void setdx(float _dx)
		{
			dx = _dx;
		}
		float getdy()
		{
			return dy;
		}
		void setdy(float _dy)
		{
			dy = _dy;
		}
		FloatRect getRect() {
			return FloatRect(x, y, w, h);
		}
		void setImage(Image &image)
		{
			texture.loadFromImage(image); //загружаем название файла изображения
			sprite.setTexture(texture);
		}
		string getname()
		{
			return name;
		}
		//get/set для других атрибутов, если понадобится
		virtual void update(float time) = 0;
		void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
		{
			for (int i = 0; i < obj.size(); i++)//проходимся по объектам
				if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
				{
					if (obj[i].name == "solid")//если встретили препятствие
					{
						if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0;}
						if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
						if (Dx > 0) { x = obj[i].rect.left - w; }
						if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
					}
				}
		}
		void сheckCollisionWithShell(Image &Image_rules, bool &touch_shell)
		{
			if (!touch_shell)
			{
				for (int i = 0; i < obj.size(); i++)//проходимся по объектам
					if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
					{
						if (obj[i].name == "Shell")//если встретили препятствие
						{
							touch_shell = true;
							Texture texture_rules;
							Sprite sprite_rules;
							texture_rules.loadFromImage(Image_rules); //загружаем название файла изображения
							sprite_rules.setTexture(texture_rules);
							sprite_rules.setOrigin(obj[i].rect.width / 2, obj[i].rect.height / 2);
							Font font;//шрифт 
							font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
							Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
							text.setFillColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
							text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
							text.setString("Для выхода из подсказки нажмите Enter");//задает строку тексту
							text.setPosition(view.getCenter().x - 165, view.getCenter().y - 200);//задаем позицию текста, центр камеры
							window.draw(text);//рисую этот текст
							while (true)
							{
								int n = 0;
								if (sprite_rules.getPosition() != Vector2f(100,15))
								{
									n += 2;
									sprite_rules.setPosition(obj[i].rect.width / 2, obj[i].rect.height / 2 + 0.1);
								}
								if (Keyboard::isKeyPressed(Keyboard::Enter)) break;
							}
							text.setString("");
							break;
						}
					}
			}
		}
		//движение камеры setPlayerCoordinateForView
		void setPlayerCoordinateForView(float x, float y) //функция для считывания координат игрока
		{ 
			float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края
			//числа поменять
			if (x < 0) tempX = 0;//убираем из вида левую сторону
			if (y < 29) tempY = 29;//верхнюю сторону
			if (y > 199) tempY = 199;//нижнюю сторону	
			//в конце добавить правую сторону
			view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
		}
};

class Shark;
class Flock;
class Jellyfish;
class Other_Fish;

class Fish : public Entity
{
public:
	Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, string _name): Entity(image, X, Y, W, H, _name)
	{
		obj = lev.GetAllObjects();
	}
	virtual void update(float time) = 0;
};

class My_Fish : public Fish
{
	bool touch_shell;
	Image image_rules_fish;
public:
	My_Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev) : Fish(image, X, Y, W, H, lev, "My_Fish")
	{
		touch_shell = false;
		image_rules_fish.loadFromFile("images/rules_fish.png");
	};
	void control(float time)
	{
		if (getlife())
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				setstate(1);
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				setstate(0);
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				setstate(3);
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				setstate(2);
			}
			setspeed(0.1);
			switch (getn_rect())
			{
			case 1:
			{
				setx(90); sety(220); setw(150); seth(110);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(2);
				break;
			}
			case 2:
			{
				setx(240); sety(220); setw(150); seth(110);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(3);
				break;
			}
			case 3:
			{
				setx(390); sety(210); setw(158); seth(115);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(4);
				break;
			}
			case 4:
			{
				setx(0); sety(330); setw(160); seth(110);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(5);
				break;
			}
			case 5:
			{
				setx(160); sety(330); setw(150); seth(108);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(6);
				break;
			}
			case 6:
			{
				setx(310); sety(325); setw(152); seth(114);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(7);
				break;
			}
			case 7:
			{
				setx(0); sety(440); setw(160); seth(110);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(8);
				break;
			}
			case 8:
			{
				setx(160); sety(436); setw(150); seth(114);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(9);
				break;
			}
			case 9:
			{
				setx(310); sety(431); setw(150); seth(119);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(1);
				break;
			}
			}
		}
	}
	void update(float time)
	{
		control(time);//функция управления персонажем
		switch (getstate())//тут делаются различные действия в зависимости от состояния
		{
		case STATE_RIGHT:setdx(getspeed()); break;//состояние идти вправо
		case STATE_LEFT: setdx(-getspeed()); break;//состояние идти влево
		case STATE_UP:setdy(getspeed()); break;//будет состояние поднятия наверх
		case STATE_DOWN: setdy(-getspeed()); break;//будет состояние во время спуска персонажа	
		}
		setx(getx() + getdx() * time);
		checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
		sety(gety() + getdy() * time);
		checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
		sprite.setPosition(getx() + getw() / 2, gety() + geth() / 2); //задаем позицию спрайта в место его центра
		сheckCollisionWithShell(image_rules_fish, touch_shell);
		if (getlife()) { setPlayerCoordinateForView(getx(), gety()); }
	}
};

class Other_Fish : public Fish
{
	bool flock;
	int number;
	float x_rand;
	float y_rand;
	int pos;
	bool escape;
public:
	Other_Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, int pos, string _name) :Fish(image, X, Y, W, H, lev, _name)
	{
		flock = false;
		escape = false;
		number = 0;
		x_rand = getx();
		y_rand = gety();
	}
	void setflock(bool _flock)
	{
		flock = _flock;
	}
	bool getflock()
	{
		return flock;
	}
	void setnumber(int _number)
	{
		number = _number;
	}
	void Move(float time)
	{
		if (pos == 1)
		{
			for (int i = 0; i < obj.size(); i++)
				if (obj[i].name == "My_Shark")
				{
					if (obj[i].rect.left - getx() <= 20) //20????
					{
						escape = true;
						float dist_min = 300;
						float x_min = 199;
						float y_min = 29;
						for (int j = 0; j < obj.size(); j++)
						{
							if (obj[j].name == "Algae")
							{
								if (dist_min > (abs(obj[j].rect.left - getx())))
								{
									dist_min = abs(obj[j].rect.left - getx());
									x_min = obj[j].rect.left;
									y_min = obj[j].rect.height;
								}
							}
						}
						x_rand = x_min;
						y_rand = y_min;
					}
					else escape = false;
					break;
				}
		}
		if ((!flock))
		{
			if ((x_rand >= getx()) && (y_rand >= gety()) && (!escape))
			{
				x_rand = rand() % 200;
				y_rand = rand() % 30;
			}
			if (x_rand > getx())
			{
				setstate(1);
				setdx(getspeed());
			}
			else
			{
				setstate(0);
				setdx(-getspeed());//0.1???
			}
			setx(getx() + getdx()*time);
			checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
			if (y_rand > gety())
			{
				setstate(2);
				setdy(getspeed()); //0.1???
			}
			else
			{
				setstate(3);
				setdy(-getspeed());
			}
			sety(gety() + getdy()*time);
			checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
		}
		else if (pos == 2)
		{
			for (int i = 0; i < obj.size(); i++)
				if (obj[i].name == "My_Fish")
				{
					if (number % 3 == 0)
					{
						setx(obj[i].rect.left - obj[i].rect.width*(number / 3));
						sety(obj[i].rect.top);
					}
					else if (number % 3 == 1)
					{
						setx(obj[i].rect.left - obj[i].rect.width*(number / 3));
						sety(obj[i].rect.top - obj[i].rect.height);
					}
					else
					{
						setx(obj[i].rect.left - obj[i].rect.width*(number / 3));
						sety(obj[i].rect.top + obj[i].rect.height);
					}
				}
			checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
			checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
		}
		sprite.setPosition(getx() + getw() / 2, gety() + geth() / 2);
	}
	virtual void update(float time) = 0;
};

class Simple_Fish : public Other_Fish
{
public:
	Simple_Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, int pos) :Other_Fish(image, X, Y, W, H, lev, pos, "Simple_Fish")
	{
	}
	void update(float time)
	{
		if (getlife())
		{
			Move(time);
			switch (getn_rect())
			{
			case 1:
			{
				setx(0); setw(260);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(2);
				break;
			}
			case 2:
			{
				setx(260); setw(250);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(3);
			}
			case 3:
			{
				setx(510); setw(260);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(4);
				break;
			}
			case 4:
			{
				setx(0); setw(250);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(5);
				break;
			}
			case 5:
			{
				setx(1020);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(1);
				break;
			}
			}
		}
		else
		{
			sprite.setPosition(getx() + 10, gety() + 10);
			setx(getx() + 10);
			sety(gety() + 10);
			setlife(true);
			setflock(false);
		}
	}
};

class Leave_Fish : public Other_Fish
{
	int time_flock;
public:
	Leave_Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, int pos) :Other_Fish(image, X, Y, W, H, lev, pos, "Leave_Fish")
	{
		time_flock = 20;    //20????
	}
	void update(float time)
	{
		if (getlife() && (time_flock > 0))
		{
			Move(time);
			switch (getn_rect())
			{
			case 1:
			{
				setx(780);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(2);
				break;
			}
			case 2:
			{
				setx(880); setw(110);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(3);
			}
			case 3:
			{
				setx(990); setw(100);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(4);
				break;
			}
			case 4:
			{
				setx(1090);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(5);
				break;
			}
			case 5:
			{
				setx(1190);
				sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
				setn_rect(1);
				break;
			}
			}
			if (getflock()) time_flock -= 1;
		}
		else
		{
			sprite.setPosition(getx() + 10, gety() + 10);
			setx(getx() + 10);
			sety(gety() + 10);
			setlife(true);
			time_flock = 20;
			setflock(false);
		}
	}
};

class Colour_Fish : public Other_Fish
{
	float x_shark;
	int colour;
	Image image_blue, image_violet, image_red, image_green;
public:
	Colour_Fish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, int pos) :Other_Fish(image, X, Y, W, H, lev, pos, "Colour_Fish")
	{
		for (int i = 0; i < obj.size(); i++)
			if (obj[i].name == "Shark") x_shark = abs(obj[i].rect.left - getx());
		colour = 0;
		image_blue.loadFromFile("images/colour_fish_blue.png");
		image_violet.loadFromFile("images/colour_fish_violet.png");
		image_red.loadFromFile("images/colour_fish_red.png");
		image_green.loadFromFile("images/colour_fish_green.png");
	}
	void update(float time)
	{
		if (getlife())
		{
			Move(time);
			if (getflock())
			{
				if (x_shark < 20)
				{
					colour = 0;
					setImage(image_green);
					//setTextureRect(IntRect(/*новые значения*/));   //20-50????? умножаем на colour
				}
				else if ((x_shark >= 20) && (x_shark < 50))
				{
					colour = 1;
					setImage(image_blue);
					//sprite.setTextureRect(IntRect(/*новые значения*/));
				}
				else if ((x_shark >= 50) && (x_shark < 70))
				{
					colour = 2;
					setImage(image_violet);
					//sprite.setTextureRect(IntRect(/*новые значения*/));
				}
				else
				{
					colour = 3;
					setImage(image_red);
				}
			}
		}
		else
		{
			sprite.setPosition(getx() + 10, gety() + 10);
			setx(getx() + 10);
			sety(gety() + 10);
			setlife(true);
			colour = 0;
			setImage(image_green);
			//sprite.setTextureRect(IntRect(/*новые значения*/));
			setflock(false);
		}
		switch (getn_rect())
		{
		case 1:
		{
			setx(0); setw(540); sety(0);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(2);
			break;
		}
		case 2:
		{
			setx(540);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(3);
		}
		case 3:
		{
			setx(1080);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(4);
			break;
		}
		case 4:
		{
			setx(1610);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(5);
			break;
		}
		case 5:
		{
			setx(2150); setw(550);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(6);
			break;
		}
		case 6:
		{
			setx(0); setw(540); sety(410);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(7);
			break;
		}
		case 7:
		{
			setx(540);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(8);
		}
		case 8:
		{
			setx(1080);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(9);
			break;
		}
		case 9:
		{
			setx(1610);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(10);
			break;
		}
		case 10:
		{
			setx(2150); setw(550);
			sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
			setn_rect(1);
			break;
		}
		}
	}
};

class Flock
{
	float x;
	float y;
	float w;
	float h;
	vector <Fish*> flock;
public:
	Flock(My_Fish *f)
	{
		x = f->getx();
		y = f->gety();
		w = f->getw();
		h = f->geth();
		flock.push_back(f);
	}
	void AddFish(Other_Fish *f)
	{
		flock.push_back(f);
		x = f->getx();
		y = f->gety();
		w = f->getw();
		h = f->geth();
		f->setflock(true);
		f->setnumber(flock.size());
	}
	void DissapearFish()
	{
		flock.back()->setlife(false);
		flock.pop_back();
		x = flock.back()->getx();
		y = flock.back()->gety();
		w = flock.back()->getw();
		h = flock.back()->geth();
	}
	int getx()
	{
		return x;
	}
	int gety()
	{
		return y;
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
};

class Shark : public Entity
{
	public:
		Shark(Image &image, float X, float Y, float W, float H, /*TileMap*/  Level &lev, string _name) :Entity(image, X, Y, W, H, _name)
		{
			obj = lev.GetAllObjects();
		}
		bool Eat(float _x, float _y, float _w, float _h, Entity *e)
		{
			//for (int i = 0; i < obj.size(); i++)
			//if (getRect().intersects(obj[i].rect))
			//{
			//	if (((obj[i].name == "Leave_Fish") || (obj[i].name == "Simple_Fish") || (obj[i].name == "Colour_Fish") || (obj[i].name == "My_Fish")))
			if(getRect().intersects(e->getRect()))
			{
					bool flag = true;
					for (int j = 0; j < obj.size(); j++)
						if ((obj[j].name == "Algae") && (obj[j].rect == e->getRect()))
						{
							flag = false;
								break;
						}
					if (flag)
					{
						sprite.setTextureRect(IntRect(_x, _y, _w, _h));
						e->setlife(false);
						return true;
					}
				//}

			}
			return false;
		}
		bool JellyFishAttack()
		{
			for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "JellyFish")
				return true;
			}
			return false;
		}
		void CheckCollisionWithAlgae(float Dx, float Dy)
		{
			for (int i = 0; i < obj.size(); i++)//проходимся по объектам
				if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
				{
					if (obj[i].name == "Algae")//если встретили препятствие
					{
						if (Dy > 0) { sety(obj[i].rect.top - geth());  setdy(0); }
						if (Dy < 0) { sety(obj[i].rect.top + obj[i].rect.height);   setdy(0); }
						if (Dx > 0) { setx(obj[i].rect.left - getw()); }
						if (Dx < 0) { setx(obj[i].rect.left + obj[i].rect.width); }
					}
				}
		}
		virtual void update(float time) = 0;
};

class My_Shark : public Shark
{
	int health;
	bool eat;
	bool touch_shell;
	
	Image image_rules_shark;
	public:
		My_Shark(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev) :Shark(image, X, Y, W, H, lev, "My_Shark")
		{
			health = 100;
			touch_shell = false;
			image_rules_shark.loadFromFile("images/rules_shark.png");
			eat = false;
		}
		void seteat(bool _eat)
		{
			eat = _eat;
		}
		bool SharkAttack(Entity *s)
		{
			if ((getRect().intersects(s->getRect())) && (s->getstate() == getstate()))
			{
				sprite.setTextureRect(IntRect(240, 290, 360, 205));
				s->setlife(false);
				sprite.setTextureRect(IntRect(0, 180, 242, 280));
				return true;
			}
			return false;
		}
		void control()
		{
			if (getlife())
			{
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					setstate(1);
				}

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					setstate(0);
				}

				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					setstate(3);
				}

				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					setstate(2);
				}
				setspeed(0.1);
				switch (getn_rect())
				{
					case 1:
					{
						setx(0); sety(0); setw(290); seth(140);
						sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
						setn_rect(2);
						break;
					}
					case 2:
					{
						setx(300); sety(0); setw(300); seth(190);
						sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
						setn_rect(1);
						break;
					}
				}
			}
		}
		void update(float time)
		{
			control();//функция управления персонажем
			switch (getstate())//тут делаются различные действия в зависимости от состояния
			{
			case STATE_RIGHT:setdx(-getspeed()); break;//состояние идти вправо
			case STATE_LEFT: setdx(getspeed()); break;//состояние идти влево
			case STATE_UP:setdy(getspeed()); break;//будет состояние поднятия наверх
			case STATE_DOWN: setdy(-getspeed()); break;//будет состояние во время спуска персонажа	
			}
			setx(getx() + getdx() * time);
			checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
			CheckCollisionWithAlgae(getdx(), 0);
			sety(gety() + getdy() * time);
			checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
			CheckCollisionWithAlgae(0, getdy());
			sprite.setPosition(getx() + getw() / 2, gety() + geth() / 2); //задаем позицию спрайта в место его центра
			сheckCollisionWithShell(image_rules_shark,touch_shell);
			if (!eat) health--;
			else
			{
				health++;
				eat = false;
			}
			if (JellyFishAttack() || health <= 0) setlife(false);
			if (getlife()) { setPlayerCoordinateForView(getx(), gety()); }
		}
};

class Enemy_Shark : public Shark
{
	int x_fish;
	int y_fish;
	int gamelevel;
	public:
		Enemy_Shark(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev, int _gamelevel) :Shark(image, X, Y, W, H, lev, "Enemy_Shark")
		{
			x_fish = getx();
			y_fish = gety();
			gamelevel = _gamelevel;
		}
		void setxy_fish(Flock *f)
		{
			x_fish = f->getx();
			y_fish = f->gety();
		}
		bool Eat_Flock(Flock *f)
		{
			if (getRect().intersects(f->getRect()))
			{

				sprite.setTextureRect(IntRect(1360, 1310, -1540, 1430));
				f->DissapearFish();
				setn_rect(3);
				//sprite.setTextureRect(IntRect(1620, 1290, -1820, 1440));

			}
			return false;
		}
		void Move(float time)
		{
			if (gamelevel == 1)
			{
				if (x_fish <= getx() && y_fish <= gety())
				{
					for (int i = 0; i < obj.size(); i++)
						if ((obj[i].name == "Leave_Fish") || (obj[i].name == "Simple_Fish") || (obj[i].name == "Colour_Fish") || (obj[i].name == "My_Fish"))
						{
							if (obj[i].rect.left > getx())
							{
								x_fish = obj[i].rect.left;
								y_fish = obj[i].rect.top;
								break;
							}
						}
				}
			}
			if (x_fish > getx())
			{
				setstate(1);
				setdx(getspeed());
			}
			else
			{
				setstate(0);
				setdx(-getspeed());//0.1???
			}
			setx(getx() + getdx()*time);
			checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
			CheckCollisionWithAlgae(getdx(), 0);
			if (y_fish > gety())
			{
				setstate(2);
				setdy(getspeed()); //0.1???
			}
			else
			{
				setstate(3);
				setdy(-getspeed());
			}
			sety(gety() + getdy()*time);
			checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
			CheckCollisionWithAlgae(0, getdy());
			sprite.setPosition(getx() + getw() / 2, gety() + geth() / 2);
		}
		void update(float time)
		{
			if (JellyFishAttack()) setlife(false);
			if (getlife())
			{
				Move(time);
				switch (getn_rect())
				{
					case 1: 
					{
						setx(1390); sety(1130); setw(160); seth(160);
						sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
						setn_rect(2);
						break;
					}
					case 2: 	
					{
						setx(1670); sety(1120); setw(130); seth(130);
						sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
						setn_rect(1);
						break;
					}
					case 3:
					{
						setx(1360); sety(1310); setw(180); seth(120);
						sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
						setn_rect(1);
						break;
					}
				}
			}
			else
			{
				sprite.setPosition(getx() + 10, gety() + 10);
				setlife(true);
				setx(getx()+10);
				sety(gety() + 10);
				setn_rect(1);
				x_fish = getx();
				y_fish = gety();
			}
		}
};


class JellyFish : public Entity
{
	float x_rand;
	float y_rand;
	bool shark_attack;
	int attack;
	int big;
	public:
		JellyFish(Image &image, float X, float Y, float W, float H, /*TileMap*/ Level &lev): Entity(image, X, Y, W, H, "JellyFish")
		{
			obj = lev.GetAllObjects();
			x_rand = getx();
			y_rand = gety();
			shark_attack = false;
			attack = 0;
			big = 1;
		}
		void Move(float time)
		{
			for (int i = 0; i < obj.size(); i++)
				if (getRect().intersects(obj[i].rect))
					if (obj[i].name == "My_Fish")
					{
						shark_attack = true;
						attack = 20;   //20?????????????
						break;
					}
			if (!shark_attack)
			{
				if ((x_rand >= getx()) && (y_rand >= gety()))
				{
					x_rand = rand() % 200;
					y_rand = rand() % 30;
				}
			}
			else
			{
				for (int j = 0; j < obj.size(); j++)
					if (obj[j].name == "Enemy_Shark")
					{
						x_rand = obj[j].rect.left;
						y_rand = obj[j].rect.top;
						break;
					}
			}
			if (x_rand > getx())
			{
				setstate(1);
				setdx(getspeed());
			}
			else
			{
				setstate(0);
				setdx(-getspeed());//0.1???
			}
			setx(getx() + getdx()*time);
			checkCollisionWithMap(getdx(), 0);//обрабатываем столкновение по Х
			if (y_rand > gety())
			{
				setstate(2);
				setdy(getspeed()); //0.1???
			}
			else
			{
				setstate(3);
				setdy(-getspeed());
			}
			sety(gety() + getdy()*time);
			checkCollisionWithMap(0, getdy());//обрабатываем столкновение по Y
			sprite.setPosition(getx() + getw() / 2, gety() + geth() / 2);
		}
		void update(float time)
		{
			Move(time);
			if (attack > 0) attack--;
			else shark_attack = false;
			switch (getn_rect())
			{
				case 1:
				{
					setx(1130); sety(1580); setw(160); seth(190);
					sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
					big = 1;
					setn_rect(getn_rect()+big);
					break;
				}
				case 2:
				{
					setx(680); sety(1280); setw(300); seth(210);
					sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
					setn_rect(getn_rect() + big);
				}
				case 3:
				{
					setx(1050); sety(1180); setw(460); seth(290);
					sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
					setn_rect(getn_rect() + big);
					break;
				}
				case 4:
				{
					setx(1540); sety(1550); setw(440); seth(330);
					sprite.setTextureRect(IntRect(getx(), gety(), getw(), geth()));
					big = -1;
					setn_rect(getn_rect() + big);
					break;
				}
			}
		}
};

class Game
{
	int gamelevel;
	/*TileMap*/ Level lvl;
	int gameover;
	public:
		Game()
		{
			gamelevel = 0;
			gameover = 0;
			lvl.LoadFromFile("map.tmx");
		}
		void menu1()
		{
			Texture menuTexture1, menuTexture2, menuBackground;
			menuTexture1.loadFromFile("images/111.png");
			menuTexture2.loadFromFile("images/333.png");
			//menuTexture3.loadFromFile("images/333.png");
			//aboutTexture.loadFromFile("images/about.png");
			menuBackground.loadFromFile("images/background_menu.jpg");
			Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
			bool isMenu = 1;
			int menuNum = 0;
			menu1.setOrigin(85, 20); 
			menu2.setOrigin(50, 20);
			menu1.setPosition(300, 150);
			menu2.setPosition(300, 250);
			//menu3.setPosition(100, 150);
			menuBg.setPosition(0, 0);

			//////////////////////////////МЕНЮ///////////////////
			while (isMenu)
			{
				menu1.setColor(Color::White);
				menu2.setColor(Color::White);
				//menu3.setColor(Color::White);
				menuNum = 0;
				window.clear(Color(129, 181, 221));

				if (IntRect(300, 150, 85, 20).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Cyan); menuNum = 1; }
				if (IntRect(300, 250, 50, 20).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Cyan); menuNum = 2; }
				//if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
					//if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
					if (menuNum == 2) { window.close(); isMenu = false; }

				}

				window.draw(menuBg);
				window.draw(menu1);
				window.draw(menu2);
				//window.draw(menu3);

				window.display();
			}
		}

		void menu2()
		{
			Texture menuTexture1, menuTexture2, menuBackground;
			menuTexture1.loadFromFile("images/fish_menu.png");
			menuTexture2.loadFromFile("images/shark_menu.png");
			menuBackground.loadFromFile("images/background_menu.jpg");
			Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
			bool isMenu = 1;
			int menuNum = 0;
			menu1.setOrigin(100, 60);
			menu2.setOrigin(100, 80);
			menu1.setPosition(200, 250);
			menu2.setPosition(500, 250);
			menuBg.setPosition(0, 0);
			Font font;//шрифт 
			font.loadFromFile("madelikesslab.ttf");//передаем нашему шрифту файл шрифта
			Text text1("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
			text1.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
			text1.setStyle(Text::Bold);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
			text1.setPosition(150, 60);//задаем позицию текста, центр камеры
			text1.setString("Choose your hero!");//задает строку тексту
			while (isMenu)
			{
				menu1.setColor(Color::White);
				menu2.setColor(Color::White);
				//menu3.setColor(Color::White);
				menuNum = 0;
				window.clear(Color(129, 181, 221));

				if (IntRect(100, 60, 200, 250).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Cyan); menuNum = 1; }
				if (IntRect(320, 80, 500, 250).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Cyan); menuNum = 2; }
				//if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (menuNum == 1) { gamelevel = 1;  isMenu = false; }//если нажали первую кнопку, то играеем за акулу
					//if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
					if (menuNum == 2) { gamelevel = 2;  isMenu = false; } //играем за рыбку

				}

				window.draw(menuBg);
				window.draw(menu1);
				window.draw(menu2);
				window.draw(text1);//рисую этот текст
				//window.draw(menu3);

				window.display();
			}
		}

		void gameover_menu()
		{
			Texture menuTexture1, menuBackground;
			menuTexture1.loadFromFile("images/bubble_menu.png");
			menuBackground.loadFromFile("images/background_menu.jpg");
			Sprite menu1(menuTexture1), menuBg(menuBackground);
			bool isMenu = 1;
			int menuNum = 0;
			menu1.setPosition(view.getCenter().x, view.getCenter().y); //поправить
			menuBg.setPosition(345, 0);
			Font font;//шрифт 
			font.loadFromFile("madelikesslab.ttf");//передаем нашему шрифту файл шрифта
			Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
			text.setFillColor(Color::White);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
			text.setStyle(Text::Bold | Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
			text.setString("MENU");//задает строку тексту
			text.setPosition(menu1.getOrigin().x, menu1.getOrigin().y);//задаем позицию текста, центр камеры
			Text text1("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
			text1.setFillColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
			text1.setStyle(Text::Bold | Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
			text1.setPosition(menu1.getOrigin().x, menu1.getOrigin().y + 50);//задаем позицию текста, центр камеры
			if (gameover < 0)
			{
				text1.setString("YOU LOSE!");//задает строку тексту	
			}
			else
			{
				text1.setString("YOU WIN!");//задает строку тексту
			}
			Text text_gameover("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
			text_gameover.setFillColor(Color::Magenta);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
			text_gameover.setStyle(Text::Bold | Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
			text_gameover.setString("GAME OVER");//задает строку тексту
			text_gameover.setPosition(menu1.getOrigin().x, menu1.getOrigin().y+100);//задаем позицию текста, центр камеры
			while (isMenu)
			{
				menu1.setColor(Color::White);
				menuNum = 0;
				window.clear(Color(129, 181, 221));

				if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Cyan); menuNum = 1; }
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (menuNum == 1) { gameover = 0;  isMenu = false; }//если нажали первую кнопку, то играеем за акулу
				}
				window.draw(menuBg);
				window.draw(menu1);
				window.draw(text);//рисую этот текст
				window.draw(text1);//рисую этот текст
				window.draw(text_gameover);//рисую этот текст
				window.display();
			}
		}

		void this_game(float time)
		{
			Image image_simplefish, image_colourfish, image_leavefish, image_jellyfish, image_enemyshark;
			image_simplefish.loadFromFile("images/simple_fish.png");
			image_colourfish.loadFromFile("images/colour_fish_green.png");
			image_leavefish.loadFromFile("images/leave_fish.png");
			image_jellyfish.loadFromFile("images/jellyfish.png");
			image_enemyshark.loadFromFile("images/enemy_shark.png");
			list<Entity*>  entities;
			list<Entity*>::iterator it;
			//list<Entity*>::iterator it2;//второй итератор.для взаимодействия между объектами списка
			vector<Object> e = lvl.GetObjects("Simple_Fish");
			for (int i = 0; i < e.size(); i++) entities.push_back(new Simple_Fish(image_simplefish, e[i].rect.left, e[i].rect.top, 200, 97, lvl, gamelevel));
			e = lvl.GetObjects("Colour_Fish");
			for (int i = 0; i < e.size(); i++) entities.push_back(new Colour_Fish(image_colourfish, e[i].rect.left, e[i].rect.top, 200, 97, lvl, gamelevel));
			e = lvl.GetObjects("Leave_Fish");
			for (int i = 0; i < e.size(); i++) entities.push_back(new Leave_Fish(image_leavefish, e[i].rect.left, e[i].rect.top, 200, 97, lvl, gamelevel));
			e = lvl.GetObjects("Jelly_Fish");
			for (int i = 0; i < e.size(); i++) entities.push_back(new JellyFish(image_jellyfish, e[i].rect.left, e[i].rect.top, 200, 97, lvl));
			Object enemy = lvl.GetObject("Enemy_Shark");
			Enemy_Shark *enemyshark = new Enemy_Shark(image_enemyshark, enemy.rect.left, enemy.rect.top, 200, 97, lvl, gamelevel);
			Object finishshell = lvl.GetObject("Finish_Shell");
			if (gamelevel == 1)
			{
				Image image_myshark;
				Image image_rules_shark;
				image_rules_shark.loadFromFile("images/rules_shark.png");
				image_myshark.loadFromFile("images/my_shark.png");
				Object player = lvl.GetObject("hero");
				My_Shark *myshark = new My_Shark(image_myshark, player.rect.left, player.rect.top, 200, 97, lvl);
				while ((myshark->getlife()) && (myshark->getRect().intersects(finishshell.rect)))
				{
					myshark->update(time);
					for (it = entities.begin(); it != entities.end(); ++it)
					{
						if ((*it)->getname() == "Simple_Fish"|| (*it)->getname() == "Colour_Fish" || (*it)->getname() == "Leave_Fish")
						{
							if (myshark->Eat(0,180,242,460,(*it)))
							{
								myshark->seteat(true);
								(*it)->update(time);
								myshark->update(time);
							}
							if (enemyshark->Eat(1360,1310,-1540,1430, (*it)))
							{
								enemyshark->setn_rect(3);
								(*it)->update(time);
								enemyshark->update(time);
							}

						}
						if ((*it)->getname() == "JellyFish")
						{
							(*it)->update(time);
						}
					}
					myshark->SharkAttack(enemyshark);
					enemyshark->update(time);
					window.setView(view);
					window.clear(Color(77, 83, 140));
					lvl.Draw(window);
					//window.draw(lvl);
					for (it = entities.begin(); it != entities.end(); ++it) {
						window.draw((*it)->sprite);
					}
					window.draw(myshark->sprite);
					window.draw(enemyshark->sprite);
					window.display();
				}
				if (myshark->getlife())
				{
					gameover = 1;
				}
				else gameover = -1;	
				delete myshark;
			}
			else
			{
				Image image_myfish;
				image_myfish.loadFromFile("images/my_fish.png");
				Object player = lvl.GetObject("Hero");
				My_Fish *myfish = new My_Fish(image_myfish, player.rect.left, player.rect.top, 200, 97, lvl);
				Flock *flock = new Flock(myfish);
				while ((myfish->getlife()) && (myfish->getRect().intersects(finishshell.rect)))
				{
					myfish->update(time);
					enemyshark->Eat_Flock(flock);
					enemyshark->setxy_fish(flock);
					enemyshark->update(time);
					for (it = entities.begin(); it != entities.end(); ++it)
					{
						if ((*it)->getname() == "Simple_Fish" || (*it)->getname() == "Colour_Fish" || (*it)->getname() == "Leave_Fish")
						{
							(*it)->update(time);
						}
						if ((*it)->getname() == "JellyFish")
						{
							(*it)->update(time);
						}
					}
					window.setView(view);
					window.clear(Color(77, 83, 140));
					//window.draw(lvl);
					lvl.Draw(window);
					for (it = entities.begin(); it != entities.end(); it++) {
						window.draw((*it)->sprite);
					}
					window.draw(myfish->sprite);
					window.draw(enemyshark->sprite);
					window.display();
				}
				if (myfish->getlife())
				{
					gameover = 1;
				}
				else gameover = -1;
				delete myfish;
				delete flock;
			}

			delete enemyshark;
			for (it = entities.begin(); it != entities.end(); ++it)
			{
				entities.erase(it);
			}
			gameover_menu();
		}
};



int main()
{
	Clock clock;
	float time = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	time = time / 800;
	Game *game = new Game();
	while (window.isOpen())
	{
		game->menu1();
		game->menu2();
		game->this_game(time);
	}
	delete game;
	return 0;
}