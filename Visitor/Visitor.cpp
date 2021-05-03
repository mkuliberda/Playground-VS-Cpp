// Visitor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "model.hpp"
#include "visitor.hpp"
#include <algorithm>
#include <sstream>

using namespace std;

struct HtmlVisitor :Visitor {

	void visit(const Paragraph& p) override {
		oss << "<p>" << p.text << "</p>\n";
	}

	void visit(const ListItem& li) override {
		oss << "<li>" << li.text << "</li>\n";
	}

	void visit(const List& l) override {
		oss << "<ul>\n";
		for (const auto& item : l) {
			item.accept(*this);
		}
		oss << "</ul>\n";
	}

	string str() const override {
		return oss.str();
	}

private:
	ostringstream oss;
};

struct MarkdownVisitor :Visitor {

	void visit(const Paragraph& p) override {
		oss << "P " << p.text << "\n";
	}

	void visit(const ListItem& li) override {
		oss << "L " << li.text << "\n";
	}

	void visit(const List& l) override {
		oss << "U \n";
		for (const auto& item : l) {
			item.accept(*this);
		}
		oss << "\n";
	}

	string str() const override {
		return oss.str();
	}

private:
	ostringstream oss;
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Paragraph p{ "some colors" };
	ListItem red{ "red" };
	ListItem green{ "green" };
	ListItem blue{ "blue" };

	List list{ red, green, blue };
	vector<Element*> document{ &p, &list };
	HtmlVisitor v;


	for (auto item : document) {
		item->accept(v);
	}
	cout << v.str() << endl;

	MarkdownVisitor m;
	for (auto item : document) {
		item->accept(m);
	}
	cout << m.str() << endl;


	getchar();
	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
