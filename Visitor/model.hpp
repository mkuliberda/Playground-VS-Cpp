#pragma once
#include <string>
#include <vector>
#include "visitor.hpp"
using namespace std;

//struct Visitor;

struct Element
{
	virtual ~Element() = default;
	virtual void accept(Visitor& v) const = 0;
};

struct TextMessage : Element
{
	string text;

	explicit TextMessage(const string& text)
		: text(text)
	{
	}
};

struct Paragraph : TextMessage
{
	explicit Paragraph(const string& text)
		: TextMessage(text)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct ListItem : TextMessage
{
	explicit ListItem(const string& text)
		: TextMessage(text)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct List : vector<ListItem>, Element
{
	List(const initializer_list<value_type>& _Ilist)
		: vector<ListItem>(_Ilist)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};