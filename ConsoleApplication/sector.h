#pragma once
#include <string>
#include <iostream>
#include "main.h"
#include <vector>
/**
 * It makes sense to use the Builder pattern only when your sectors are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated sectors. In other words, results of various builders may not
 * always follow the same interface.
 */

class Sector{
    public:
    std::vector<std::string> parts_;
    void ListParts()const{
        std::cout << "Sector parts: ";
        for (size_t i=0;i<parts_.size();i++){
            if(parts_[i]== parts_.back()){
                std::cout << parts_[i];
            }else{
                std::cout << parts_[i] << ", ";
            }
        }
        std::cout << "\n\n"; 
    }
	~Sector() {
		std::cout << "Sector dtor" << std::endl;
	}
};


/**
 * The Builder interface specifies methods for creating the different parts of
 * the Sector objects.
 */
class Builder{
    public:
    virtual ~Builder(){}
    virtual void ProducePartA() const =0;
    virtual void ProducePartB() const =0;
    virtual void ProducePartC() const =0;
};
/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class SectorBuilder : public Builder{
    private:

	std::unique_ptr<Sector> sector;

    /**
     * A fresh builder instance should contain a blank sector object, which is
     * used in further assembly.
     */
    public:

    SectorBuilder(){
        this->Reset();
    }

    ~SectorBuilder(){
		std::cout << "SectorBuilder dtor" << std::endl;
        //delete sector;
    }

    void Reset(){
		this->sector.reset(new Sector);
    }
    /**
     * All production steps work with the same sector instance.
     */

    void ProducePartA()const override{
        this->sector->parts_.push_back("PartA1");
    }

    void ProducePartB()const override{
        this->sector->parts_.push_back("PartB1");
    }

    void ProducePartC()const override{
        this->sector->parts_.push_back("PartC1");
    }

    /**
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different sectors that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language). Note that PHP is a
     * dynamically typed language and this method CAN be in the base interface.
     * However, we won't declare it there for the sake of clarity.
     *
     * Usually, after returning the end result to the client, a builder instance
     * is expected to be ready to start producing another sector. That's why
     * it's a usual practice to call the reset method at the end of the
     * `getProduct` method body. However, this behavior is not mandatory, and
     * you can make your builders wait for an explicit reset call from the
     * client code before disposing of the previous result.
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct the user of this function is responsible to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks
     */

    std::unique_ptr<Sector> GetProduct() {
		std::unique_ptr<Sector>(result);
		result = std::move(this->sector);
        this->Reset();
        return result;
    }
};

