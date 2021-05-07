#include "gtest/gtest.h"
#include "common.h"
#include "stdio.h"

TEST(Serialization, packingToJSON){
    struct Drawing go;
    std::string outS = go.packToJSON();
    struct Drawing back; 
    back.unpackFromJSON(outS);
    std::cout << "out struct:" << go.toString() << std::endl; 
    std::cout << "out string:" << outS << std::endl; 

    std::cout << "out struct:" << back.toString() << std::endl; 

    
    EXPECT_EQ(5, 5);
}


TEST(Serialization, packingToBinary){
    struct Drawing go;
    std::string outS = go.packToBinary();
    struct Drawing back; 



    std:: istringstream is(outS, std::stringstream::binary);        
    cereal::PortableBinaryInputArchive unArch(is);
    unArch(back);


    // back.unpackFromBinary(outS);
    std::cout << "in struct:" << go.toString() << std::endl; 

    std::cout << "out struct:" << back.toString() << std::endl; 

    
    EXPECT_EQ(5, 5);
}

