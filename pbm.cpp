#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/* Reads ascii file of 1s and 0s and the writes a binary file. */

struct PBM
{
    unsigned int width;
    unsigned int height;
    std::string comment;
    std::vector<std::vector<char>> data;

    bool ReadAscii(std::ifstream& f)
    {
        std::string id;
        if(!std::getline(f, id) || id != "P1")
        {
            return false;
        }
        if(f.peek() == '#' && !std::getline(f, comment))
        {
            return false;
        }
        if(!(f >> width >> height))
        {
            return false;
        }
        data.resize(height);
        int temp;
        for(size_t y = 0; y < data.size(); ++y)
        {
            data[y].resize(width);
            for(size_t x = 0; x < width; ++x)
            {
                if(!(f >> temp))
                {
                    return false;
                }
                data[y][x] = static_cast<char>(temp);
            }
        }
        return true;
    }

    bool WriteBinary(std::ofstream& f)
    {
        if(!(f << "P4\n"))
        {
            return false;
        }
        if(!comment.empty() && !(f << comment << "\n"))
        {
            return false;
        }
        if(!(f << width << " " << height << "\n"))
        {
            return false;
        }
        // std::vector<char> linebits((width + (CHAR_BIT - 1)) / CHAR_BIT);
        for(size_t y = 0; y < data.size(); ++y)
        {
            // std::fill(linebits.begin(), linebits.end(), 0);
            // for(size_t x = 0; x < width; ++x)
            // {
            //     const int byte_pos = x / CHAR_BIT;
            //     const int bit_pos = (CHAR_BIT - 1) - (x % CHAR_BIT);
            //     linebits[byte_pos] |= (data[y][x] << bit_pos);
            // }
            // if(!f.write(&linebits[0], linebits.size()))
            // {
            //     return false;
            // }
        }
        return true;
    }
    // bool WriteBinary(std::ofstream& f)
    // {
    //     if(!(f << "P4\n"))
    //     {
    //         return false;
    //     }
    //     if(!comment.empty() && !(f << comment << "\n"))
    //     {
    //         return false;
    //     }
    //     if(!(f << width << " " << height << "\n"))
    //     {
    //         return false;
    //     }
    //     std::vector<char> linebits((width + (CHAR_BIT - 1)) / CHAR_BIT);
    //     for(size_t y = 0; y < data.size(); ++y)
    //     {
    //         std::fill(linebits.begin(), linebits.end(), 0);
    //         for(size_t x = 0; x < width; ++x)
    //         {
    //             const int byte_pos = x / CHAR_BIT;
    //             const int bit_pos = (CHAR_BIT - 1) - (x % CHAR_BIT);
    //             linebits[byte_pos] |= (data[y][x] << bit_pos);
    //         }
    //         if(!f.write(&linebits[0], linebits.size()))
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
};

int main(int argc, char * argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: convertpbm <ascii filename> <binary filename>\n";
        return 0;
    }

    PBM pbm;

    std::ifstream inFile(argv[1]);
    if(!inFile)
    {
        std::cerr << "Could not open '" << argv[1] << "' for input!\n";
        return 0;
    }
    if(!pbm.ReadAscii(inFile))
    {
        std::cerr << "Error reading input file!\n";
    }

    std::ofstream outFile(argv[2], std::ios::binary);
    if(!outFile)
    {
        std::cerr << "Could not open '" << argv[1] << "' for output!\n";
        return 0;
    }
    if(!pbm.WriteBinary(outFile))
    {
        std::cerr << "Error writing output file!\n";
    }

    return 0;
}