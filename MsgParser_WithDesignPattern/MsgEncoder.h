#pragma once
#include <string>
#include <vector>
#include "Messages.h"

struct List;
struct ListItem;
struct Paragraph;

struct Visitor
{
	virtual ~Visitor() = default;

	virtual void visit(const Paragraph& p) = 0;
	virtual void visit(const ListItem& p) = 0;
	virtual void visit(const List& p) = 0;

	virtual std::string str() const = 0;
};

struct Element
{
	virtual ~Element() = default;
	virtual void accept(Visitor& v) const = 0;
};

struct TextMessage : Element
{
	std::string text;

	explicit TextMessage(const std::string& text)
		: text(text)
	{
	}
};

struct Paragraph : TextMessage
{
	explicit Paragraph(const std::string& text)
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
	explicit ListItem(const std::string& text)
		: TextMessage(text)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct List : std::vector<ListItem>, Element
{
	List(const std::initializer_list<value_type>& _Ilist)
		: std::vector<ListItem>(_Ilist)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

