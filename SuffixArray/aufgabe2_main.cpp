#include <iostream>
#include <string>
#include <vector>
#include "aufgabe2.hpp"


int main(int argc, char* argv[])
{
    std::vector<uint32_t> sa;
    if (argc == 2)
    {
        std::string text(argv[1]);
        construct(sa, text);
        for (unsigned i = 0; i < sa.size(); i++)
        {
            std::cout << sa[i] << std::endl;
        }
    }

    else if (argc > 2)
    {
        
       
        std::string text(argv[1]);
        construct(sa, text);
        int laenge(argc);
        for (int i = 2; i < laenge; i++)
        {
           
            std::string query(argv[i]);
            std::vector<uint32_t> hits;
            std::cout << query << ": ";
            find(query, sa, text, hits);
           
            
            for (unsigned i = 0; i < hits.size(); i++)
            {
               
                std::cout << hits[i] << " ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cerr << "unexpected input" << std::endl;
        return 1;
    }


    
}

