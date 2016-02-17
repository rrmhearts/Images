#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>

const int NIBBLE_SIZE = 4;
const char binary[16][5] = { "0000", "0001", "0010", "0011",
							 "0100", "0101", "0110", "0111",
							 "1000", "1001", "1010", "1011",
							 "1100", "1101", "1110", "1111"
};
char hexArray[] = "0123456789ABCDEF";

char* bytes2Hex(unsigned char * bytes, size_t len_bytes) {
	char* hexChars = (char*)malloc(2 * len_bytes + 1);
	int j;
	for (j = 0; j < len_bytes; j++) {
		int v = bytes[j] & 0xFF;
		hexChars[j * 2] = hexArray[v >> 4];
		hexChars[j * 2 + 1] = hexArray[v & 0x0F];
	}
	hexChars[j * 2] = 0;
	return hexChars;
}
struct PBM
{
	unsigned int width;
	unsigned int height;
	std::string comment;
	std::vector<std::vector<char>> data;


	void print()
	{
		printf("PBM info, width: %d, height: %d", width, height);
	}
	bool ReadAscii(std::ifstream& f)
	{
		std::string id;
		if (!std::getline(f, id) || id != "P1")
		{
			return false;
		}
		if (f.peek() == '#' && !std::getline(f, comment))
		{
			return false;
		}
		if (!(f >> width >> height))
		{
			return false;
		}
		data.resize(height);
		int temp;
		for (size_t y = 0; y < data.size(); ++y)
		{
			data[y].resize(width);
			for (size_t x = 0; x < width; ++x)
			{
				if (!(f >> temp))
				{
					return false;
				}
				data[y][x] = static_cast<char>(temp);
			}
		}
		return true;
	}

	bool WriteHex(std::ofstream& f)
	{
		//if (!(f << "P1\n"))
		//{
		//	return false;
		//}
		//if (!comment.empty() && !(f << comment << "\n"))
		//{
		//	return false;
		//}
		//if (!(f << width << " " << height << "\n"))
		//{
		//	return false;
		//}
		std::vector<char> linehex(width / NIBBLE_SIZE);
		
		std::vector<char> linebits((width + (CHAR_BIT - 1)) / CHAR_BIT);
		for(size_t y = 0; y < data.size(); ++y)
		{
			std::fill(linehex.begin(), linehex.end(), 0);
			std::fill(linebits.begin(), linebits.end(), 0);
			for(size_t x = 0; x < width; ++x)
			{
				const int byte_pos = x / CHAR_BIT;
				const int bit_pos = (CHAR_BIT - 1) - (x % CHAR_BIT);
				linebits[byte_pos] |= (data[y][x] << bit_pos);
			}
			int hex_index = 0;
			std::string hextwo;
			std::cout << "bits size: " << linebits.size() << " linehex size: " << linehex.size() << std::endl;
			//std::string str(linebits.begin(), linebits.end());
			//std::cout << str << std::endl;
			for(size_t x = 0; x < linebits.size(); x++)
			{
				//std::cout << "line x: " << linebits[x]+0x30 << "  x+1: " << linebits[x+1]+0x30 << std::endl;
				//char bits_8 = linebits[x];
				//std::ostringstream ss;
				//std::cout << '0' + ((linebits[x] & 0xF0)>>4) << '0' + (linebits[x] & 0x0F) << std::endl;
				//ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)	<< static_cast<unsigned>(linebits[x]);
				//hextwo = ss.str();//<< std::hex << linebits[x];
//				char* bytes2Hex(unsigned char * bytes, size_t len_bytes) {
				hextwo = bytes2Hex((unsigned char *)&linebits[x], 1);
				//std::cout << hextwo << std::endl;
				//hextwo[2] = 0;
				//sprintf_s(hextwo, "%02X", bits_8);
				linehex[hex_index++] = hextwo[0];
				//const char * pstr = str.substr(x, x + NIBBLE_SIZE).c_str();
				//std::cout << str.substr(x, x + NIBBLE_SIZE) << std::endl;
				//std::cout << "bits: " << strtol(pstr, NULL, 2) << std::endl;
				//linehex[hex_index] =bits.to_string();
				linehex[hex_index++] = hextwo[1];
				std::cout << hextwo << "   ind: " << hex_index << "  x: "<< x << std::endl;
				
			}
			//std::cin.get();
			//linehex[hex_index] = 0;
			//std::cout << "here" << std::endl;
			f << "B," << y << ",0, H, \"";
			if(!f.write(&linehex[0], linehex.size()))
			{
			     return false;
			}
			f << "\" | " << std::endl;
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
	if (argc != 3)
	{
		std::cerr << "Usage: convertpbm <ascii filename> <binary filename>\n";
		return 0;
	}

	PBM pbm;

	std::ifstream inFile(argv[1]);
	if (!inFile)
	{
		std::cerr << "Could not open '" << argv[1] << "' for input!\n";
		return 0;
	}
	if (!pbm.ReadAscii(inFile))
	{
		std::cerr << "Error reading input file!\n";
	}
	pbm.print();

	std::ofstream outFile(argv[2]);
	if (!outFile)
	{
		std::cerr << "Could not open '" << argv[1] << "' for output!\n";
		return 0;
	}
	if (!pbm.WriteHex(outFile))
	{
		std::cerr << "Error writing output file!\n";
	}

	return 0;
}