/*
This is zlib License.
 
 Original code by Tetsuro KANI

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must
  not claim that you wrote the original software. If you use this
  software in a product, an acknowledgment in the product documentation
  would be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and
  must not be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source
  distribution.
*/

#ifndef LIB_BMP24_HPP
#define LIB_BMP24_HPP

#define LIBBMP_VERSION      1002
#define LIBBMP_SIGNATURE    19778

#include <cstdlib>
#include <cstdint>
#include <fstream>


namespace libbmp24 {


class Bitmap {

private:
    //  Bitmap File Header
    //   not use signature area.
    //   because unuse "#pragma pack" for portable code. 
    struct FileHeader {
        uint32_t size_;
        uint16_t reserved1_;
        uint16_t reserved2_;
        uint32_t offbits_;

        void setup(const int image_size) {
            size_ = image_size + sizeof(FileHeader) + sizeof(InfoHeader);
            reserved1_ = 0;
            reserved2_ = 0;
            offbits_ = 2+sizeof(FileHeader) + sizeof(InfoHeader);
        }
    };

    //  Bitmap Information Header.
    struct InfoHeader{
        uint32_t size_;
        int32_t width_;
        int32_t heigth_;
        uint16_t planes_;
        uint16_t bit_count_;
        uint32_t compression_;
        uint32_t size_image_;
        int32_t pels_per_meter_x_;
        int32_t pels_per_meter_y_;
        uint32_t clr_used_;
        uint32_t clr_important_;

        //  make information header.
        void setup(
            const int width,
            const int height
        ){
            int padding = (width * 3 + 3) / 4 * 4 - width * 3;    // 32bit check
            int img_size = (width * 3 + padding) * height;

            size_ = sizeof(InfoHeader);
            width_ = width;
            heigth_ = height;
            planes_ = 1;
            bit_count_ = 24;
            compression_ = 0;
            size_image_ = img_size;
            pels_per_meter_x_ = 96;
            pels_per_meter_y_ = 96;
            clr_used_ = 0;
            clr_important_ = 0;
        }



    };

public:
    //  ctor.
    Bitmap()
        : file_header_()
        , info_header_()
        , data_(NULL)
    {}

    //  dtor.
    ~Bitmap() {
        delete[] data_;
    }

    //  width getter.
    int32_t getWidth() const {
        return info_header_.width_;
    }

    //  hight getter.
    int32_t getHeight() const {
        return info_header_.heigth_;
    }

    //  image size getter. byte.
    uint32_t getImageSize() const {
        return info_header_.size_image_;
    }

    //  pixel data address getter.
    const uint8_t* getData() const {
        return data_;
    }

    //  dump to console.
    void dump() const {
    
        printf("libbmp24::dump\n");
        printf(" size = %d\n", info_header_.size_);
        printf(" width = %d\n", info_header_.width_);
        printf(" heigth = %d\n", info_header_.heigth_);
        printf(" planes = %d\n", info_header_.planes_);
        printf(" bit_count = %d\n", info_header_.bit_count_);
        printf(" compression = %d\n", info_header_.compression_);
        printf(" size_image = %d\n", info_header_.size_image_);
        printf(" pels_per_meter_x = %d\n", info_header_.pels_per_meter_x_);
        printf(" pels_per_meter_y = %d\n", info_header_.pels_per_meter_y_);
        printf(" clr_used = %d\n", info_header_.clr_used_);
        printf(" clr_important = %d\n", info_header_.clr_important_);
        
        if (data_) {
            for (int x = 0; x < info_header_.width_; ++x) {
                for (int y = 0; y < info_header_.heigth_; ++y) {
                    int pixel_idx = x + (y * info_header_.width_);
                    printf( " %3d %3d %3d\n", data_[pixel_idx + 0], data_[pixel_idx + 1], data_[pixel_idx + 2]);
                }
            }
        }
        else {
            printf(" no color data.\n");
        }
    }

    //  fill bitmap all pixel.
    void fill(int r, int g, int b) {
        int width = getWidth();
        int height = getHeight();
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                plot(x, y, r, g, b);
            }
        }
    }

    //  plot.
    void plot(
        uint32_t x, 
        uint32_t y,
        int r,
        int g, 
        int b
    ) {
        //  check range.
        int width = getWidth();
        if (x > width || y > getHeight()) {
            return;
        }

        //  plot.
        int ny = getHeight()-1-y;

        int pixel_idx = x + (ny * width);
        int data_idx = pixel_idx * 3;

        data_[data_idx + 0] = static_cast<uint8_t>(b);
        data_[data_idx + 1] = static_cast<uint8_t>(g);
        data_[data_idx + 2] = static_cast<uint8_t>(r);        
    }


private:
    FileHeader file_header_;
    InfoHeader info_header_;
    uint8_t* data_;

public:
    //  create new bitmap.
    void createBitmap(
        int width,
        int height
    ) {
        //  make information header.
        info_header_.setup(width, height);
        int image_size = getImageSize();

        //  make file header.
        file_header_.setup(image_size);

        //  data strage.
        delete[] data_;
        data_ = new uint8_t[image_size];
    }


    //  serialize bitmap.
    bool serialize(std::ofstream& file) const {
        //  save header.
        uint16_t bm_signature = LIBBMP_SIGNATURE;
        file.write((char*)&bm_signature, 2);
        file.write((char*)&file_header_, sizeof(FileHeader));
        file.write((char*)&info_header_, sizeof(InfoHeader));
        //  save data.
        file.write((char*)data_, getImageSize());
        //  fin.
        return true;
    }

    bool serialize(std::ostringstream& file) const {
        //  save header.
        uint16_t bm_signature = LIBBMP_SIGNATURE;
        file.write((char*)&bm_signature, 2);
        file.write((char*)&file_header_, sizeof(FileHeader));
        file.write((char*)&info_header_, sizeof(InfoHeader));
        //  save data.
        file.write((char*)data_, getImageSize());
        //  fin.
        return true;
    }

    bool serialize(std::stringstream& file) const {
        //  save header.
        uint16_t bm_signature = LIBBMP_SIGNATURE;
        file.write((char*)&bm_signature, 2);
        file.write((char*)&file_header_, sizeof(FileHeader));
        file.write((char*)&info_header_, sizeof(InfoHeader));
        //  save data.
        file.write((char*)data_, getImageSize());
        //  fin.
        return true;
    }

    //  validate check.
    bool isVaildate() const {
        if (info_header_.size_ != sizeof(InfoHeader)) {
            return false;
        }
        if (info_header_.compression_ != 0) {
            return false;
        }

        int img_size = info_header_.size_image_;
        int x = info_header_.width_;
        int y = info_header_.heigth_;
        int color_bit = info_header_.bit_count_;

        //  support 24bit color format only.
        if (color_bit  != 24) {
            return false;
        }

        if (color_bit  == 0) {
            return false;
        }

        if (x <= 0 || y <= 0) {
            return false;
        }

        if (img_size != file_header_.size_ - file_header_.offbits_) {
            return false;
        }

        return true;
    }

    //  deserialize bitmap.
    bool deserialize (
        std::ifstream& file
    ) {

        //  signature
        uint16_t signature = 0;
        file.read((char*)(&signature), 2);

        if (signature != LIBBMP_SIGNATURE) {
            return false;
        }

        file.read((char *)(&file_header_), sizeof(FileHeader));
        if( file.bad() ) {
            return false;
        }

        file.read((char *)(&info_header_), sizeof(InfoHeader));
        if (file.bad()) {
            return false;
        }


        if (!isVaildate()) {
            return false;
        }

        int img_size = getImageSize();
        delete[] data_;
        data_ = new uint8_t[img_size];

        if (!data_) {
            return false;
        }

        file.read((char *)data_, img_size);

        return true;
    }
};



}   // namespace libbmp24


#endif // LIB_BMP24_HPP

