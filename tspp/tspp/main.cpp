#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <list>
#include <vector>
using namespace std;
#define ENTER 13

struct dateformat
{
	int d;
	int m;
	int y;
};

class Product
{
	string type;
	string name;
	int number;
	int price;
	bool availability;
	public:
		Product(string _type, string _name, int _number, int _price)
		{
			type = _type;
			name = _name;
			number = _number;
			price = _price;
			availability = true;
		}
		string gettype()
		{
			return type;
		}
		void settype(string _type)
		{
			type = _type;
		}
		string getname()
		{
			return name;
		}
		void setname(string _name)
		{
			name = _name;
		}
		int getnumber()
		{
			return number;
		}
		void setnumber(int _number)
		{
			number = _number;
		}
		int getprice()
		{
			return price;
		}
		void setprice(int _price)
		{
			price = _price;
		}
		bool getavailability()
		{
			return availability;
		}
		void setavailability(bool _availability)
		{
			availability = _availability;
		}
};

class Catalogue_Product
{
	list <Product*> *products;
	public:
		Catalogue_Product() 
		{
			products = new list <Product*>;
		}
		void addlistProduct(Product *p)
		{
			products->push_back(p);
		}
		list <Product*> *get_products()
		{
			return products;
		}
		bool search_product(string _type)
		{
			for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
			{
				if ((*it)->gettype() == _type)
				{
					return true;
				}
			}
			return false;
		}
		list <Product*> *search_product_type(string _type)
		{
			list <Product*> *product_type = new list <Product*>;
			for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
			{
				if ((*it)->gettype() == _type)
				{
					product_type->push_back((*it));
				}
			}
			return product_type;
		}
		bool search_product_name(string _name)
		{
			if (!products->empty())
			{
				for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
				{
					if ((*it)->getname() == _name)
					{
						return true;
					}
				}
			}
			return false;
		}
		Product *give_product_name(string _name)
		{
			for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
			{
				if ((*it)->getname() == _name)
				{
					return (*it);
				}
			}
		}
		list <Product*> *search_product_unavailable()
		{
			list <Product*> *pr = new list <Product*>;
			for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
			{
				if (!(*it)->getavailability())
				{
					pr->push_back((*it));
				}
			}
			return pr;
		}
		int search_price(string _name)
		{
			for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
			{
				if ((*it)->getname() == _name)
				{
					return (*it)->getprice();
				}
			}
			return 0;
		}
		~Catalogue_Product()
		{
			delete products;
		}
};

class Bill
{
	string FIO;
	int ID;
	dateformat date;
	vector <pair<string, int>> *products;
	public:
		Bill(string _FIO, int _ID, dateformat _date, vector <pair<string, int>> *_products)
		{
			FIO = _FIO;
			ID = _ID;
			date = _date;
			products = _products;
		}
		string getFIO()
		{
			return FIO;
		}
		void setFIO(string _FIO)
		{
			FIO = _FIO;
		}
		int getID()
		{
			return ID;
		}
		void setID(int _ID)
		{
			ID = _ID;
		}
		dateformat getdate()
		{
			return date;
		}
		void setdate(dateformat _date)
		{
			date = _date;
		}
		vector <pair<string, int>> *getproduct()
		{
			return products;
		}
		void add_product(string _name, int _number)
		{
			products->push_back(pair<string,int>(_name, _number));
		}
		~Bill()
		{
			delete products;
		}
};

class HistoryBuying
{
	list<Bill*> *bills;
	public:
		HistoryBuying() 
		{
			bills = new list<Bill*>;
		}
		list<Bill*> *getbills()
		{
			return bills;
		}
		void AddlistBill(Bill *_bill)
		{
			bills->push_back(_bill);
		}
		int searchdate(string _name, dateformat _date, int flag)
		{
			int k = 0;
			for (list<Bill*>::iterator it = bills->begin(); it != bills->end(); ++it)
			{
				for (vector<pair<string, int>>::iterator it1 = (*it)->getproduct()->begin(); it1 != (*it)->getproduct()->end(); ++it1)
				{
					if (it1->first == _name)
					{
						switch (flag)
						{
							case 1: 
							{
								if (((*it)->getdate().d  <= _date.d + 7) && ((*it)->getdate().m == _date.m) && ((*it)->getdate().y == _date.y))
								{
									k += it1->second;
								}
								break;
							}
							case 2:
							{
								if (((*it)->getdate().m == _date.m) && ((*it)->getdate().y == _date.y))
								{
									k += it1->second;
								}
								break;
							}
							case 3:
							{
								if ((*it)->getdate().y == _date.y)
								{
									k += it1->second;
								}
								break;
							}
						}
					}
				}
				
			}
			return k;
		}
		bool searchID(int _ID)
		{
			for (list<Bill*>::iterator it = bills->begin(); it != bills->end(); ++it)
			{
				if ((*it)->getID() == _ID)
				{
					return true;
				}
			}
			return false;
		}
		vector <pair<string, int>> *searchproduct(int _ID)
		{
			for (list<Bill*>::iterator it = bills->begin(); it != bills->end(); ++it)
			{
				if ((*it)->getID() == _ID)
				{
					return (*it)->getproduct();
				}
			}
		}
		~ HistoryBuying()
		{
			delete bills;
		}
};

class Order
{
	int ID;
	int number_order;
	Product *order_product;
	public:
		Order(int _ID, int _number_order, string _type, string _name, int _number, int _price)
		{
			ID = _ID;
			number_order = _number_order;
			order_product = new Product(_type, _name, _number, _price);
		}
		Order(int _ID, int _number_order, Product *_product)
		{
			ID = _ID;
			number_order = _number_order;
			order_product = _product;
		}
		int getID()
		{
			return ID;
		}
		int getnumber_order()
		{
			return number_order;
		}
		Product *getorder_product()
		{
			return order_product;
		}
		~Order()
		{
			delete order_product;
		}
};

class Director
{
	public:
		Director() {}
		void View_Product(string _type, Catalogue_Product *catalogue)
		{
			if (catalogue->search_product(_type)) 
			{
				list <Product*> *pr = catalogue->search_product_type(_type);
				cout << "Тип товаров: " << _type << endl;
				for (list<Product*>::iterator it = pr->begin(); it != pr->end(); ++it)
				{
					cout << "Информация о товаре:" << endl;
					cout << (*it)->getname() << endl;
					cout << "Цена:" << (*it)->getprice() << endl;
					if ((*it)->getavailability())
					{
						cout << "Товар есть в наличии" << endl;
						cout << "Количество товара:" << (*it)->getnumber() << endl;
					}
					else cout << "Товара нет в наличии" << endl;
				}
				//delete pr;
			}
			else cout << "Товара с таким видом нет в каталоге" << endl;
		}
		void View_Product_Unavailable(Catalogue_Product *catalogue)
		{
			list <Product*> *pr = catalogue->search_product_unavailable();
			cout << "Товары не в наличии" << endl;
			for (list<Product*>::iterator it = pr->begin(); it != pr->end(); ++it)
			{
				cout << "Информация о товаре:" << endl;
				cout << "Тип товара:" << (*it)->gettype() << endl;
				cout << "Название товара:" << (*it)->getname() << endl;
				cout << "Цена:" << (*it)->getprice() << endl;
			}
			delete pr;
		}
		void Order_Product(string _name, int _number_order, Catalogue_Product *catalogue, list <Order*> *catalogue_orders)
		{
			int _ID;
			if (catalogue_orders->empty())
			{
				_ID = 1;
			}
			else _ID = catalogue_orders->back()->getID() + 1;
			if (catalogue->search_product_name(_name))
			{
				Order *ord = new Order(_ID, _number_order, catalogue->give_product_name(_name));
				catalogue_orders->push_back(ord);
			}
			else
			{
				string _type;
				int _price;
				cout << "Введите более подробную информацию" << endl;
				cout << "Введите тип товара:" << endl;
				cin >> _type;
				cout << "Введите цену товара:" << endl;
				cin >> _price;
				Order *ord = new Order(_ID, _number_order,  _type, _name, 0, _price);
				catalogue_orders->push_back(ord);
			}
		}
		void ViewStatistics(int flag, string _type, dateformat _date, HistoryBuying *historybuying, Catalogue_Product *catalogue)
		{
			int k = 0;
			if (catalogue->search_product(_type))
			{
				list<Product*> *products = catalogue->search_product_type(_type);
				for (list<Product*>::iterator it = products->begin(); it != products->end(); ++it)
				{
					k += historybuying->searchdate((*it)->getname(), _date, flag);
				}
				cout << "Товар типа " << _type << " был продан в количестве " << k;
				switch (flag)
				{
				case 1:   cout << " за неделю"; break;
				case 2:   cout << " за месяц"; break;
				case 3:   cout << " за год"; break;
				}
				cout << " от ";
				if (_date.d < 10) cout << "0";
				cout << _date.d << ".";
				if (_date.m < 10) cout << "0";
				cout << _date.m << ".";
				if (_date.y < 10) cout << "0";
				cout << _date.y << endl;
			}
			else cout << "Такого типа товаров нет в каталоге" << endl;
		}
}; 

class StoreWorker
{
	public:
	StoreWorker() {}
	void View_Product(string _name, Catalogue_Product *catalogue)
	{
		if (catalogue->search_product_name(_name))
		{
			Product *pr = catalogue->give_product_name(_name);
			cout << "Название товара: " << _name << endl;
			cout << "Информация о товаре:" << endl;
			if (pr->getavailability())
			{
				cout << "Товар есть в наличии" << endl;
				cout << "Количество товара:" << pr->getnumber() << endl;
			}
			else cout << "Товара нет в наличии" << endl;
			delete pr;
		}
		else cout << "Товара с таким названием нет в каталоге" << endl;
	}
	void Changenumber_Product(Order *ord, Catalogue_Product *catalogue)
	{
		int n = catalogue->give_product_name(ord->getorder_product()->getname())->getnumber();
		catalogue->give_product_name(ord->getorder_product()->getname())->setnumber(n + ord->getnumber_order());
		catalogue->give_product_name(ord->getorder_product()->getname())->setavailability(true);
	}
	void RecieveProduct(int _ID, list <Order*> *catalogue_orders, Catalogue_Product *catalogue)
	{

		for (list<Order*>::iterator it = catalogue_orders->begin(); it != catalogue_orders->end(); ++it)
		{
			if ((*it)->getID() == _ID)
			{
				cout << "Информация о заказе:" << endl;
				cout << "Название товара: "<<(*it)->getorder_product()->getname() << endl;
				cout << "Тип товара: " << (*it)->getorder_product()->gettype() << endl;
				cout << "Цена товара: " << (*it)->getorder_product()->getprice() << endl;
				cout << "Количество товара на складе: " << (*it)->getorder_product()->getnumber() << endl;
				cout << "Количество товара в заказе: " << (*it)->getnumber_order() << endl;
				cout << "Забрать заказ?  (1-Да/2-Нет)" << endl;
				int n;
				cin >> n;
				if (n==1)
				{
					if (!catalogue->search_product_name((*it)->getorder_product()->getname()))
					{
						cout << "Товара нет в каталоге" << endl;
						catalogue->addlistProduct((*it)->getorder_product());
						cout << "Товар добавлен в каталог" << endl;
					}
					Changenumber_Product((*it), catalogue);
					cout << "Обновлённая информация о товаре:" << endl;
					cout << "Название товара: " << (*it)->getorder_product()->getname() << endl;
					cout << "Тип товара: " << (*it)->getorder_product()->gettype() << endl;
					cout << "Цена товара: " << (*it)->getorder_product()->getprice() << endl;
					cout << "Количество товара на складе: " << (*it)->getorder_product()->getnumber() << endl;
					catalogue_orders->erase(it);
					cout << "Заказ удалён из списка" << endl;
				}
				return;
			}
		}
		cout << "Заказа с таким ID нет" << endl;
		
	}
};

class Seller
{
	public:
		Seller(){}
		void CreateBill(HistoryBuying *historybuying, string _FIO, int _ID, dateformat _date, vector <pair<string, int>> *_products)
		{
			Bill *bill = new Bill(_FIO, _ID, _date,_products);
			historybuying->AddlistBill(bill);
		}
};

class Cashier
{
	public:
		Cashier(){}
		int setprice(string _name, int _number, Catalogue_Product *catalogue)
		{
			return catalogue->search_price(_name)*_number;
		}
		void MakeCheque(int _ID, Catalogue_Product *catalogue, HistoryBuying *historybuying)
		{
			if (historybuying->searchID(_ID))
			{
				vector <pair<string, int>> *_products = historybuying->searchproduct(_ID);
				cout << "Чек №" << _ID << endl;
				int s = 0;
				for (vector<pair<string, int>>::iterator it = _products->begin(); it != _products->end(); ++it)
				{
					cout << it ->first << "..............................." << setprice(it->first, it->second, catalogue) << endl;
					int n = catalogue->give_product_name(it->first)->getnumber();
					catalogue->give_product_name(it->first)->setnumber(n - it->second);
					if (catalogue->give_product_name(it->first)->getnumber() == 0) catalogue->give_product_name(it->first)->setavailability(false);
					s += setprice(it->first, it->second, catalogue);
				}
				cout << "Итого к оплате:  " << s << endl;
				//delete _products;
			}
			else cout << "Накладной с таким ID не найдено" << endl;
		}
};

class Console
{
	Catalogue_Product *catalogue;
	HistoryBuying *historybuying;
	list <Order*> *catalogue_orders;
	public:
		Console()
		{
			catalogue = new Catalogue_Product();
			historybuying = new HistoryBuying();
			catalogue_orders = new list <Order*>();
		}
		int Menu_Mode()
		{
			system("cls");
			cout << "Кто будет использовать программу? (введите цифру 1-5)" << endl;
			cout << "1 - Руководство" << endl;
			cout << "2 - Работник склада" << endl;
			cout << "3 - Продавец" << endl;
			cout << "4 - Кассир" << endl;
			cout << "5 - Выйти из программы" << endl;
			int n;
			cin >> n;
			return n;
		}
		int Menu_Director()
		{
			system("cls");
			cout << "Что Вы хотите сделать (введите цифру 1-5)" << endl;
			cout << "1 - Посмотреть информацию об определённом виде товаров" << endl;
			cout << "2 - Посмотреть товары не в наличии" << endl;
			cout << "3 - Посмотреть статистику продаж товара" << endl;
			cout << "4 - Заказать товар" << endl;
			cout << "5 - Выйти в главное меню" << endl;
			int n;
			cin >> n;
			return n;
		}
		int Menu_StoreWorker()
		{
			system("cls");
			cout << "Что Вы хотите сделать (введите цифру 1-3)" << endl;
			cout << "1 - Посмотреть статус о наличии товара на складе и его количество" << endl;
			cout << "2 - Получить список заказов" << endl;
			cout << "3 - Выйти в главное меню" << endl;
			int n;
			cin >> n;
			return n;
		}
		int Menu_Seller()
		{
			system("cls");
			cout << "Хотите оформить накладную (введите цифру 1-2)" << endl;
			cout << "1 - Да" << endl;
			cout << "2 - Нет, выйти в главное меню" << endl;
			int n;
			cin >> n;
			return n;
		}
		int Menu_Cashier()
		{
			system("cls");
			cout << "Хотите оформить чек покупки (введите цифру 1-2)" << endl;
			cout << "1 - Да" << endl;
			cout << "2 - Нет, выйти в главное меню" << endl;
			int n;
			cin >> n;
			return n;
		}
		void MenuDirector(Director *director)
		{
			while (true)
			{
				switch (Menu_Director())
				{
				case 1:
				{
					string _type;
					cout << "Введите тип товаров:" << endl;
					cin >> _type;
					system("cls");
					director->View_Product(_type, catalogue);
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 2:
				{
					system("cls");
					director->View_Product_Unavailable(catalogue);
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 3:
				{
					string _type;
					dateformat _date;
					int flag;
					system("cls");
					cout << "Введите тип товара:" << endl;
					cin >> _type;
					cout << "Введите дату:" << endl;
					cout << "День ";
					cin >> _date.d;
					cout << "Месяц ";
					cin >> _date.m;
					cout << "Год ";
					cin >> _date.y;
					cout << "За какой период хотите получить статистику? (введите цифру 1-3)" << endl;
					cout << "1 - За неделю" << endl;
					cout << "2 - За месяц" << endl;
					cout << "3 - За год" << endl;
					cin >> flag;
					director->ViewStatistics(flag, _type, _date, historybuying, catalogue);
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 4:
				{
					string _name;
					int _number_order;
					system("cls");
					cout << "Введите название товара:" << endl;
					cin >> _name;
					cout << "Введите количество товара для заказа:" << endl;
					cin >> _number_order;
					director->Order_Product(_name, _number_order, catalogue, catalogue_orders);
					cout << "Товар заказан" << endl;
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 5: return;
				}
			}
		}
		void MenuStoreWorker(StoreWorker *storeworker)
		{
			while (true)
			{
				switch (Menu_StoreWorker())
				{
				case 1:
				{
					string _name;
					cout << "Введите название товара:" << endl;
					cin >> _name;
					system("cls");
					storeworker->View_Product(_name, catalogue);
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 2:
				{
					system("cls");
					if (!catalogue_orders->empty())
					{
						for (list<Order*>::iterator it = catalogue_orders->begin(); it != catalogue_orders->end(); ++it)
						{
							cout << "ID заказа: " << (*it)->getID() << endl;
							cout << "Название товара: " << (*it)->getorder_product()->getname() << endl;
							cout << "Количество заказанного товара: " << (*it)->getnumber_order() << endl;
						}
						cout << "Принять товар? (1-Да/2-Нет)" << endl;
						int n;
						cin >> n;
						if (n==1)
						{
							int _ID;
							cout << "Введите ID заказа" << endl;
							cin >> _ID;
							system("cls");
							storeworker->RecieveProduct(_ID, catalogue_orders, catalogue);
						}
					}
					else cout << "Актуальных заказов нет" << endl;
					cout << "Нажмите ENTER для выхода в меню" << endl;
					while (true)
					{
						if (_getch() == ENTER) break;
					}
					break;
				}
				case 3: return;
				}
			}
		}
		void MenuSeller(Seller *seller)
		{
			while (Menu_Seller() != 2)
			{
				string _FIO;
				int _ID;
				dateformat _date;
				vector <pair<string, int>> *_products = new vector <pair<string, int>>;
				do
				{
					cout << "Введите ID накладной (должен быть уникальным):" << endl;
					cin >> _ID;
				} while (historybuying->searchID(_ID));
				cout << "Введите ФИО покупателя:" << endl;
				cin >> _FIO;
				cout << "Введите дату:" << endl;
				cout << "День ";
				cin >> _date.d;
				cout << "Месяц ";
				cin >> _date.m;
				cout << "Год ";
				cin >> _date.y;
				cout << "Введите название и количество товара" << endl;
				while (true)
				{
					string _name;
					int _number;
					cout << "Введите название товара (для выхода - *)" << endl;
					cin >> _name;
					while ((!catalogue->search_product_name(_name)) && (!catalogue->give_product_name(_name)->getavailability()) && (_name != "*"))
					{
						cout << "Такого товара нет. " << endl;
						cout << "Введите название товара (для выхода - *)" << endl;
						cin >> _name;
					}
					if (_name == "*") break;
					cout << "Введите количество товара (для выхода - 0)" << endl;
					cin >> _number;
					while ((catalogue->give_product_name(_name)->getnumber()<_number) && (_number != 0))
					{
						cout << "Такого количества товара нет. " << endl;
						cout << "Введите количество товара (для выхода - 0)" << endl;
						cin >> _number;
					}
					if (_number == 0) break;
					_products->push_back(pair<string, int>(_name, _number));
				}
				if (_products->empty()) cout << "Накладная не оформлена!" << endl;
				else
				{
					seller->CreateBill(historybuying, _FIO, _ID, _date, _products);
					cout << "Накладная оформлена!" << endl;
				}
				cout << "Нажмите ENTER для выхода в меню" << endl;
				while (true)
				{
					if (_getch() == ENTER) break;
				}
			}
		}
		void MenuCashier(Cashier *cashier)
		{
			while (Menu_Cashier() != 2)
			{
				int _ID;
				cout << "Введите ID накладной:" << endl;
				cin >> _ID;
				cashier->MakeCheque(_ID, catalogue, historybuying);
				cout << "Нажмите ENTER для выхода в меню" << endl;
				while (true)
				{
					if (_getch() == ENTER) break;
				}
			}
		}
		void Menu()
		{
			while (true)
			{
				switch (Menu_Mode())
				{
					case 1:
					{
						Director *director = new Director();
						MenuDirector(director);
						delete director;
						break;
					}
					case 2:
					{
						StoreWorker *storeworker = new StoreWorker();
						MenuStoreWorker(storeworker);
						delete storeworker;
						break;
					}
					case 3:
					{
						Seller *seller = new Seller();
						MenuSeller(seller);
						delete seller;
						break;
					}
					case 4:
					{
						Cashier *cashier = new Cashier();
						MenuCashier(cashier);
						delete cashier;
						break;
					}
					case 5: return;
				}
			}
		}

		/*void write_file_catalogue()            
		{
			system("cls");
			ofstream t1;
			t1.open("catalogue.txt", ios::out);         
			if (!t1)                     
			{
				system("cls");
				cout << "Файл не открыт" << endl;
				cout << "введите ENTER" << endl;
				while (1)
				{
					if (_getch() == ENTER)
						return;	                   
				}
			}
			list <Product*> *pr = catalogue->get_products();
			for (list <Product*>::iterator it = pr->begin(); it != pr->end(); ++it)
			{
				t1 << (*it)->getname() << endl << (*it)->gettype() << endl << (*it)->getnumber() << endl << (*it)->getprice() << (*it)->getavailability() ;
				if ((*it) != pr->back()) t1 << endl;      
			}
			t1.close();       
		}

		void write_file_historybuying()
		{
			system("cls");
			ofstream t1;
			t1.open("historybuying.txt", ios::out);
			if (!t1)
			{
				system("cls");
				cout << "Файл не открыт" << endl;
				cout << "введите ENTER" << endl;
				while (1)
				{
					if (_getch() == ENTER)
						return;
				}
			}
			list <Bill*> *b = historybuying->getbills();
			for (list <Bill*>::iterator it = b->begin(); it != b->end(); ++it)
			{
				t1 << (*it)->getID() << endl << (*it)->getFIO() << endl << (*it)->getdate().d << endl << (*it)->getdate().m << endl << (*it)->getdate().y << endl;
				for (vector<pair<string, int>>::iterator it1 = (*it)->getproduct()->begin(); it1 != (*it)->getproduct()->end(); ++it1)
				{
					t1 << it1->first << endl << it1->second << endl;
				}
				if ((*it) != b->back()) t1 << endl;
			}
			t1.close();
		}*/

		~Console()
		{
			delete catalogue;
			delete historybuying;
			delete catalogue_orders;
		}
};

int main()
{
	SetConsoleCP(1251);                           
	SetConsoleOutputCP(1251);  
	Console *console = new Console();
	console->Menu();
	delete console;
}