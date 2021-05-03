#pragma once
#include <string>
#include <utility>
#include <vector>
#include "Messages.h"

struct Footer;
struct List;
struct ListItem;
struct Header;

struct Visitor
{
	virtual ~Visitor() = default;

	virtual void visit(const Header& p) = 0;
	virtual void visit(const ListItem& p) = 0;
	virtual void visit(const List& p) = 0;
	virtual void visit(const Footer& p) = 0;

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

	explicit TextMessage(std::string&& text)
		: text(std::move(text))
	{
	}
};

struct Header : TextMessage
{
	explicit Header(std::string&& text)
		: TextMessage(std::move(text))
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct ListItem : TextMessage
{
	explicit ListItem(std::string&& text)
		: TextMessage(std::move(text))
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct Footer : TextMessage
{
	explicit Footer(std::string&& text)
		: TextMessage(std::move(text))
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

struct List : std::vector<ListItem>, Element
{
	List(const std::initializer_list<value_type>& _ilist)
		: std::vector<ListItem>(_ilist)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}
};

