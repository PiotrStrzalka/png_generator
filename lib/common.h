#ifndef PGEN_COMMON_H
#define PGEN_COMMON_H

#include <variant>
#include <string>
#include <sstream>
#include <fstream>
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"



typedef struct{
    int x;
    int y;
    int w;
    int h;
} Rectangle;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
} Triangle;


struct Drawing {
    int screenWidth;
    int screenHeight;
    std::variant<Triangle, Rectangle> object;
    std::string name;
    Drawing() : screenHeight(100), screenWidth(100), name("bub"){};

    std::string toString()
    {
        std::stringstream ss;
        ss << "Name: " << this->name << "\n";
        ss << "Screen Width: " << this->screenWidth << "px\n";
        ss << "Screen Height: " << this->screenHeight << "px\n";
        ss << "Picture type:\n";
        if(std::holds_alternative<Rectangle>(this->object)){
            ss << " Rectangle:\n";
            ss << "  x: " << std::get<Rectangle>(this->object).x;
            ss << "  y: " << std::get<Rectangle>(this->object).y;
            ss << "  w: " << std::get<Rectangle>(this->object).w;
            ss << "  h: " << std::get<Rectangle>(this->object).h;
        } else if (std::holds_alternative<Triangle>(this->object))
        {
            ss << " Triangle:\n";
            ss << "  x1: " << std::get<Triangle>(this->object).x1;
            ss << "  y1: " << std::get<Triangle>(this->object).y1;
            ss << "  x2: " << std::get<Triangle>(this->object).x2;
            ss << "  y2: " << std::get<Triangle>(this->object).y2;
            ss << "  x3: " << std::get<Triangle>(this->object).x3;
            ss << "  y3: " << std::get<Triangle>(this->object).y3;
        } else {
            ss << "No picture type defined yet\n";
        }
        return ss.str();
    };

    bool operator==(const struct Drawing& rhs)
    {
        return (this->screenWidth == rhs.screenWidth) 
                && (this->screenHeight == rhs.screenHeight)
                && (this->name == rhs.name);
    }

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( screenWidth, screenHeight, name );
    }

    std::string packToJSON(void)
    {
        std::stringstream ss;
        cereal::JSONOutputArchive arch(ss);
        arch(*this);
        /* for some reaseon in JSON string is lack of closing bracket?*/
        return (ss.str() + "\n}");
    }

    void unpackFromJSON(std::string in)
    {
        std:: istringstream is(in);
        cereal::JSONInputArchive unArch(is);
        unArch(*this);
    }

    std::string packToBinary(void)
    {
        std::ostringstream os(std::stringstream::binary);
        {
            cereal::PortableBinaryOutputArchive arch(os);
            arch(*this);
        }
        return os.str();
    }

    std::string unpackFromBinary(std::string in)
    {
        std:: istringstream is(in, std::stringstream::binary);
        
        cereal::PortableBinaryInputArchive unArch(is);
        unArch(*this);
    }

};


// template<class InputArchive, class OutputArchive, class TypeToSerialize>
// class Archiver
// {
//     std::string serialize(TypeToSerialize object)
//     {
//         std::
//     }

//     deserialize

// } 

#endif //PGEN_COMMON_H
