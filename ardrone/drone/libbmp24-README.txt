# libbmp24

##DESCRIPTION
SIMPLE and TINY bitmap image library. This lib is one header file only.

But 24bit bitmap (true color) only.


##SAMPLES

    #include "libbmp24.hpp"

Sample 01.
Load bmp and rename save.

    int main() {
        libbmp24::Bitmap bmp;
        
        // Load bmp.
        std::ifstream is("src.bmp", std::ios::in);
        bmp.deserialize(is);
        // Rename and Save.
        std::ofstream os("dest.bmp", std::ios::out);
        bmp.serialize(os);
        return 0;
    }



Sample02.
Dump bitmap file information to console window.

    int main() {
        std::ifstream is("src.bmp", std::ios::in);
        libbmp24::Bitmap bmp;
        bmp.dump();
        return 0;
    }
    
Sample03.
Create new bitmap file.

    int main() {
        libbmp24::Bitmap bmp;
        const int width = 64;
        const int height = 100;
        bmp.createBitmap(width, height); // create new bitmap.
        bmp.fill(255, 0, 30); // r, g, b
        // save.
        std::ifstream os("dest.bmp", std::ios::in);
        bmp.deserialize(os);
        return 0;
    }
    


##LICENSE

libbmp24 is released under the zlib license.

Please read [LICENSE](https://github.com/doscoy/libbmp24/blob/master/LICENSE) file.

##CONTRIBUTERS

* Murilo Pontes.
