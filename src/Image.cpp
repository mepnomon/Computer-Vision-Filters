/**
********************************************************************************
*
*   @file       Image.cpp
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.1
*
*
*   @date       11/11/2016
*
*   @author     Franck Vidal (amended by Dorian Dressler)
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************
#define LINE_SIZE 2048


//******************************************************************************
//  Include
//******************************************************************************
#include <sstream> // Header file for stringstream
#include <fstream> // Header file for filestream
#include <algorithm> // Header file for min/max/fill
#include <cmath> // Header file for abs
#include <vector>
#include <iostream>

#include "Image.h"


//------------------
Image::Image():
//------------------
        m_width(0),
        m_height(0),
        m_p_image(0)
//------------------
{}


//----------------------------------------------
Image::Image(const Image& anImage):
//----------------------------------------------
        m_width(anImage.m_width),
        m_height(anImage.m_height),
        m_p_image(new float[m_width * m_height])
//----------------------------------------------
{
    // Out of memeory
    if (m_width && m_height && !m_p_image)
    {
        throw "Out of memory";
    }
    
    // Copy the data
    std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height, m_p_image);
}


//----------------------------------------------
Image::Image(const float* apData,
             unsigned int aWidth,
             unsigned int aHeight):
//----------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_p_image(new float[m_width * m_height])
//----------------------------------------------
{
    // Out of memeory
    if (m_width && m_height && !m_p_image)
    {
        throw "Out of memory";
    }

    // Copy the data
    std::copy(apData, apData + m_width * m_height, m_p_image);
}


//----------------------------------------------
Image::Image(unsigned int aWidth,
             unsigned int aHeight):
//----------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_p_image(new float[m_width * m_height])
//----------------------------------------------
{
    // Out of memeory
    if (m_width && m_height && !m_p_image)
    {
        throw "Out of memory";
    }

    // Initialise the data
    std::fill_n(m_p_image, m_width * m_height, 0);
}


//-------------
Image::~Image()
//-------------
{
    // Release memory
    destroy();
}


//-------------------
void Image::destroy()
//-------------------
{
    // Memory has been dynamically allocated
    if (m_p_image)
    {
        // Release the memory
        delete [] m_p_image;
    
        // Make sure the pointer is reset to NULL
        m_p_image = 0;
    }
    
    // There is no pixel in the image
    m_width  = 0;
    m_height = 0;
}


//---------------------------------------------
Image Image::getROI(unsigned int i,
                    unsigned int j,
                    unsigned int aWidth,
                    unsigned int aHeight) const
//---------------------------------------------
{
    // Create a black image
    Image roi(aWidth, aHeight);

    // Process every row of the ROI
    for (unsigned y(0); y < aHeight; ++y)
    {
        // Process every column of the ROI
        for (unsigned x(0); x < aWidth; ++x)
        {
            unsigned int index_i(x + i);
            unsigned int index_j(y +j);

            // The pixel index is valid
            if ((index_i < m_width) && (index_j < m_height))
            {
                // Get the pixel intensity from the current instance
                float intensity(getPixel(index_i, index_j));

                // Set the pixel of the ROI
                roi.setPixel(x, y, intensity);
            }
        }
    }
    
    return (roi);
}

//----------------------------------------------------------------
void Image::setPixel(unsigned int i, unsigned int j, float aValue)
//----------------------------------------------------------------
{
    // The pixel index is not valid
	//std::cout << i;
	//std::cout <<" "  <<  j << std::endl;
    if (i >= m_width || j >= m_height)
    {
        throw "set pixel Invalid pixel coordinate";
    }
    
    // Change the pixel value
    m_p_image[j * m_width + i] = aValue;
}


//---------------------------------------------------------
float Image::getPixel(unsigned int i, unsigned int j) const
//---------------------------------------------------------
{
	//std::cout << "i " << i;
	//std::cout << " j " << j << std::endl;
    // The pixel index is valid
    if (i >= m_width || j >= m_height)
    {
		std::cout << i;
		std::cout << " " << j << std::endl;
        throw "get pixel Invalid pixel coordinate";
    }
    
    // Return the pixel value
    return (m_p_image[j * m_width + i]);
}

//------------------------------------------------
bool Image::operator!=(const Image& anImage) const
//------------------------------------------------
{
	return (!(operator==(anImage)));
}

//-------------------------------------------
Image& Image::operator=(const Image& anImage)
//-------------------------------------------
{
    // The images different
    if (this != &anImage)
    {
        // Release memory
        destroy();

        // Copy the image properites
        m_width   = anImage.m_width;
        m_height  = anImage.m_height;
        m_p_image = new float[m_width * m_height];

        // Out of memeory
        if (m_width && m_height && !m_p_image)
        {
            throw "Out of memory";
        }
        
        // Copy the data
        std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height, m_p_image);
    }

    // Return the instance
    return (*this);
}


//------------------------------------------
Image Image::operator+(const Image& anImage)
//------------------------------------------
{
    // Deal with images of different sizes
    unsigned int min_width(std::min(m_width, anImage.m_width));
    unsigned int min_height(std::min(m_height, anImage.m_height));
    
    // Copy the instance into a temporary variable
    Image temp(getROI(0, 0, min_width, min_height));
    
    // Copy the data
    for (unsigned int j(0); j < min_height; ++j)
    {
        for (unsigned int i(0); i < min_width; ++i)
        {
            temp.m_p_image[j * temp.m_width + i] += anImage.m_p_image[j * anImage.m_width + i];
        }
    }
    
    // Return the result
    return (temp);
}


//------------------------------------------
Image Image::operator-(const Image& anImage)
//------------------------------------------
{
    // Deal with images of different sizes
    unsigned int min_width(std::min(m_width, anImage.m_width));
    unsigned int min_height(std::min(m_height, anImage.m_height));
    
    // Copy the instance into a temporary variable
    Image temp(getROI(0, 0, min_width, min_height));
    
    // Copy the data
    for (unsigned int j(0); j < min_height; ++j)
    {
        for (unsigned int i(0); i < min_width; ++i)
        {
            temp.m_p_image[j * temp.m_width + i] -= anImage.m_p_image[j * anImage.m_width + i];
        }
    }
    
    // Return the result
    return (temp);
}


//--------------------------------------------
Image& Image::operator+=(const Image& anImage)
//--------------------------------------------
{
    // Re-use operator+
    *this = *this + anImage;
    
    // Return the result
    return (*this);
}


//--------------------------------------------
Image& Image::operator-=(const Image& anImage)
//--------------------------------------------
{
    // Re-use operator-
    *this = *this - anImage;
    
    // Return the result
    return (*this);
}


//----------------------------------
Image Image::operator+(float aValue)
//----------------------------------
{
    // Copy the instance into a temporary variable
    Image temp(*this);

    float* p_temp(temp.m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ += aValue;
    }
    
    // Return the result
    return (temp);
}


//----------------------------------
Image Image::operator-(float aValue)
//----------------------------------
{
    // Copy the instance into a temporary variable
    Image temp(*this);

    float* p_temp(temp.m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ -= aValue;
    }
    
    // Return the result
    return (temp);
}


//----------------------------------
Image Image::operator*(float aValue)
//----------------------------------
{
    // Copy the instance into a temporary variable
    Image temp(*this);

    float* p_temp(temp.m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ *= aValue;
    }
    
    // Return the result
    return (temp);
}


//----------------------------------
Image Image::operator/(float aValue)
//----------------------------------
{
    // Division by zero
    if (std::abs(aValue) < 1.0e-6)
    {
        throw "Division by zero.";
    }
    
    // Copy the instance into a temporary variable
    Image temp(*this);

    float* p_temp(temp.m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ /= aValue;
    }
    
    // Return the result
    return (temp);
}


//-----------------------------------
Image& Image::operator+=(float aValue)
//-----------------------------------
{
    float* p_temp(m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ += aValue;
    }
    
    // Return the result
    return (*this);
}


//------------------------------------
Image& Image::operator-=(float aValue)
//------------------------------------
{
    float* p_temp(m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ -= aValue;
    }
    
    // Return the result
    return (*this);
}


//------------------------------------
Image& Image::operator*=(float aValue)
//------------------------------------
{
    float* p_temp(m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ *= aValue;
    }
    
    // Return the result
    return (*this);
}


//------------------------------------
Image& Image::operator/=(float aValue)
//------------------------------------
{
    // Division by zero
    if (std::abs(aValue) < 1.0e-6)
    {
        throw "Division by zero.";
    }
    
    float* p_temp(m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        *p_temp++ /= aValue;
    }
    
    // Return the result
    return (*this);
}


//----------------------
Image Image::operator!()
//----------------------
{
    // Copy the instance into a temporary variable
 
	Image temp(*this);
    float min_value(getMinValue());
    float max_value(getMaxValue());
    float range(max_value - min_value);
    
    float* p_temp(temp.m_p_image);
    
    // Process every pixel
    for (unsigned int i(0); i < m_width * m_height; ++i, ++p_temp)
    {
        // Take care to preserve the dynamic of the image
        *p_temp = min_value + range * (1.0 - (*p_temp - min_value) / range);
    }
    
    // Return the result
    return (temp);
}


//---------------------------------
float Image::getAspectRatio() const
//---------------------------------
{
    return (float(m_width) / float(m_height));
}


//----------------------------------
unsigned int Image::getWidth() const
//----------------------------------
{
    return (m_width);
}


//-----------------------------------
unsigned int Image::getHeight() const
//-----------------------------------
{
    return (m_height);
}


//------------------------------
float Image::getMinValue() const
//------------------------------
{
    // The image is empty
    if (!m_p_image)
    {
        throw "Empty image";
    }
    
    return (*std::min_element(&m_p_image[0], &m_p_image[m_width * m_height]));
}


//------------------------------
float Image::getMaxValue() const
//------------------------------
{
    // The image is empty
    if (!m_p_image)
    {
        throw "Empty image";
    }
    
    return (*std::max_element(&m_p_image[0], &m_p_image[m_width * m_height]));
}


//----------------------------------------------------------------
void Image::shiftScaleFilter(float aShiftValue, float aScaleValue)
//----------------------------------------------------------------
{
    // Process every pixel of the image
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        // Apply the shilft/scale filter
        m_p_image[i] = (m_p_image[i] + aShiftValue) * aScaleValue;
    }
}


//---------------------
void Image::normalize()
//---------------------
{
    shiftScaleFilter(-getMinValue(), 1.0 / (getMaxValue() - getMinValue()));
}


//----------------------------------------
void Image::loadPGM(const char* aFileName)
//----------------------------------------
{
    // Open the file
    std::ifstream input_file(aFileName, std::ifstream::binary);
    
    // The file does not exist
    if (!input_file.is_open())
    {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot open the file \"" << aFileName << "\". It does not exist";
    
        // Throw an error
        throw (error_message.str());
    }
    // The file is open
    else
    {
        // Release the memory if necessary
        destroy();
    
        // Variable to store a line
        char p_line_data[LINE_SIZE];
    
        // Get the first line
        input_file.getline(p_line_data, LINE_SIZE);

        // Get the image type
        std::string image_type(p_line_data);
    
        // Valid ASCII format
        if (image_type == "P2")
        {
            // Variable to save the max value
            int max_value(-1);
        
            // There is data to read
            unsigned int pixel_count(0);
            while (input_file.good())
            {
                // Get the new line
                input_file.getline(p_line_data, LINE_SIZE);
    
                // It is not a comment
                if (p_line_data[0] != '#')
                {
                    // Store the line in a stream
                    std::stringstream stream_line;
                    stream_line << std::string(p_line_data);
                
                    // The memory is not allocated
                    if (!m_p_image && !m_width && !m_height)
                    {
                        // Load the image size
                        stream_line >> m_width >> m_height;
                    
                        // Alocate the memory
                        m_p_image = new float[m_width * m_height];
                    
                        // Out of memory
                        if (!m_p_image)
                        {
                            throw ("Out of memory");
                        }
                    }
                    // The max value is not set
                    else if (max_value < 0)
                    {
                        // Get the max value;
                        stream_line >> max_value;
                    }
                    // Read the pixel data
                    else
                    {
                        // Process all the pixels of the line
                        while (stream_line.good())
                        {
                            // Get the pixel value
                            int pixel_value(-1);
                            stream_line >> pixel_value;
                            // The pixel exists
                            if (pixel_count < m_width * m_height)
                            {
                                m_p_image[pixel_count++] = pixel_value;
                            }
                        }
                    }
                }
            }
        }
        // Valid binary format
        else if (image_type == "P5")
        {
            // Variable to save the max value
            int max_value(-1);

            // There is data to read
            unsigned int pixel_count(0);
            while (input_file.good() && !pixel_count)
            {
            	// Process as an ASCII file
                if (!m_width || !m_height || max_value < 0)
                {
                    // Get the new line
                    input_file.getline(p_line_data, LINE_SIZE);

                    // It is not a comment
                    if (p_line_data[0] != '#')
                    {
                        // Store the line in a stream
                        std::stringstream stream_line;
                        stream_line << std::string(p_line_data);

                        // The memory is not allocated
                        if (!m_p_image && !m_width && !m_height)
                        {
                            // Load the image size
                            stream_line >> m_width >> m_height;

                            // Alocate the memory
                            m_p_image = new float[m_width * m_height];

                            // Out of memory
                            if (!m_p_image)
                            {
                                throw ("Out of memory");
                            }
                        }
                        // The max value is not set
                        else
                        {
                            // Get the max value;
                            stream_line >> max_value;
                        }
                    }
                }
                // Read the pixel data
                else
                {
                	unsigned char* p_temp(new unsigned char[m_width * m_height]);

                    // Out of memory
                    if (!p_temp)
                    {
                        throw ("Out of memory");
                    }

                    input_file.read(reinterpret_cast<char*>(p_temp), m_width * m_height);

                	for (unsigned int i(0); i < m_width * m_height; ++i)
                	{
                		m_p_image[i] = p_temp[i];
                        ++pixel_count;
                	}
                	delete [] p_temp;
                }
            }
        }
        // Invalid format
        else
        {
            // Build the error message
            std::stringstream error_message;
            error_message << "Invalid file (\"" << aFileName << "\")";

            // Throw an error
            throw (error_message.str());
        }
    }
}


//-----------------------------------------------
void Image::loadPGM(const std::string& aFileName)
//-----------------------------------------------
{
    loadPGM(aFileName.data());
}


//----------------------------------------
void Image::savePGM(const char* aFileName)
//----------------------------------------
{
    // Open the file
    std::ofstream output_file(aFileName);
    
    // The file does not exist
    if (!output_file.is_open())
    {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot create the file \"" << aFileName << "\"";
    
        // Throw an error
        throw (error_message.str());
    }
    // The file is open
    else
    {
        // Set the image type
        output_file << "P2" << std::endl;
    
        // Print a comment
        output_file << "# ICP3038 -- Assignment 1 -- 2016/2017" << std::endl;
    
        // The image size
        output_file << m_width << " " << m_height << std::endl;

        // The get the max value
        //output_file << std::min(255, std::max(0, int(getMaxValue()))) << std::endl;
        output_file << std::max(255, int(getMaxValue())) << std::endl;
    
        // Process every line
        for (unsigned int j = 0; j < m_height; ++j)
        {
            // Process every column
            for (unsigned int i = 0; i < m_width; ++i)
            {
                // Process the pixel
                int pixel_value(m_p_image[j * m_width + i]);
                pixel_value = std::max(0, pixel_value);
                pixel_value = std::min(255, pixel_value);
            
                output_file << pixel_value;
            
                // It is not the last pixel of the line
                if (i < (m_width - 1))
                {
                    output_file << " ";
                }
            }
        
            // It is not the last line of the image
            if (j < (m_height - 1))
            {
                output_file << std::endl;
            }
        }
    }
}


//-----------------------------------------------
void Image::savePGM(const std::string& aFileName)
//-----------------------------------------------
{
    savePGM(aFileName.data());
}


//----------------------------------------
void Image::loadRaw(const char* aFileName,
                    unsigned int aWidth,
                    unsigned int aHeight)
//----------------------------------------
{
    // Open the file in binary
    std::ifstream input_file (aFileName, std::ifstream::binary);

    // The file is not open
    if (!input_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") does not exist";

        throw error_message;
    }

    // Get size of file
    input_file.seekg(0, input_file.end);
    unsigned int size(input_file.tellg());

    // The size is not correct
    if (aWidth * aHeight * sizeof(float) != size)
    {
        std::stringstream error_message;
        error_message << "The size of " << aFileName << " is not " <<
                aWidth << "x" << aHeight;

        throw error_message.str();
    }

    // Rewind the file
    input_file.seekg (0);

    // Release the memory
    destroy();

    // Allocate memory for file content
    m_width = aWidth;
    m_height = aHeight;
    m_p_image = new float[m_width * m_height];

    // Read content of input_file
    input_file.read(reinterpret_cast<char*>(m_p_image), size);
}


//-----------------------------------------------
void Image::loadRaw(const std::string& aFileName,
                    unsigned int aWidth,
                    unsigned int aHeight)
//-----------------------------------------------
{
    loadRaw(aFileName.data(), aWidth, aHeight);
}


//----------------------------------------
void Image::saveRaw(const char* aFileName)
//----------------------------------------
{
    // Open the file in binary
    std::ofstream output_file (aFileName, std::ifstream::binary);

    // The file is not open
    if (!output_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw error_message;
    }

    // Write content to file
    output_file.write(reinterpret_cast<char*>(m_p_image), m_width * m_height * sizeof(float));
}


//-----------------------------------------------
void Image::saveRaw(const std::string& aFileName)
//-----------------------------------------------
{
    saveRaw(aFileName.data());
}


//------------------------------------------
void Image::loadASCII(const char* aFileName)
//------------------------------------------
{
    // Open the file
    std::ifstream input_file (aFileName);

    // The file is not open
    if (!input_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") does not exist";

        throw error_message;
    }

    // Load the data into a vector
    std::vector<float> p_data;
    std::string line;
    int number_of_rows(0);
    int number_of_columns(0);

    // Read evely line
    while (std::getline(input_file, line))
    {
        number_of_columns = 0;
        float intensity;
        std::stringstream line_parser;
        line_parser << line;
        while (line_parser >> intensity)
        {
            p_data.push_back(intensity);
            ++number_of_columns;
        }
        ++number_of_rows;
    }

    // Wrong number of pixels
    if (number_of_rows * number_of_columns != p_data.size())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") is invalid";

        throw error_message;
    }

    // Release the memory
    destroy();

    // Allocate memory for file content
    m_width = number_of_columns;
    m_height = number_of_rows;
    m_p_image = new float[m_width * m_height];

    // Copy the data
    std::copy(p_data.begin(), p_data.end(), m_p_image);
}


//-------------------------------------------------
void Image::loadASCII(const std::string& aFileName)
//-------------------------------------------------
{
    loadASCII(aFileName.data());
}


//------------------------------------------
void Image::saveASCII(const char* aFileName)
//------------------------------------------
{
    // Open the file
    std::ofstream output_file (aFileName);

    // The file is not open
    if (!output_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw error_message;
    }

    // Write content to file
    float* p_data(m_p_image);
    for (unsigned int j(0); j < m_height; ++j)
    {
        for (unsigned int i(0); i < m_width; ++i)
        {
            output_file << *p_data++;

            // This is not the last pixel of the line
            if (i < m_width - 1)
            {
                output_file << " ";
            }
        }

        // This is not the last line
        if (j < m_height - 1)
        {
            output_file << std::endl;
        }
    }


}


//-------------------------------------------------
void Image::saveASCII(const std::string& aFileName)
//-------------------------------------------------
{
    saveASCII(aFileName.data());
}


//------------------------------------------------
bool Image::operator==(const Image& anImage) const
//------------------------------------------------
{
    if (m_width != anImage.m_width)
    {
        return (false);
    }

    if (m_height != anImage.m_height)
    {
        return (false);
    }

    float const * p_data1(m_p_image);
    float const * p_data2(anImage.m_p_image);
    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        if (std::abs(*p_data1++ - *p_data2++) > 1.0e-6)
        {
            return (false);
        }
    }

    return (true);
}

//----------------------------------------------------------------
float Image::getSAE(const Image& anImage) const
//----------------------------------------------------------------
{
	if (getWidth() != anImage.getWidth() || anImage.getHeight() != anImage.getHeight())
	{
		return (false);
	}

	float const * p_data1(m_p_image);
	float const * p_data2(anImage.m_p_image);
	float sae_result(0);

	for (unsigned int i(0); i < m_width * m_height; ++i)
	{
		sae_result += (abs)(m_p_image[i] - anImage.m_p_image[i]);
	}

	return(sae_result);
}

//----------------------------------------------------------------
float Image::getNCC(const Image& anImage) const
//----------------------------------------------------------------
{
	if (getWidth() != anImage.getWidth() || anImage.getHeight() != anImage.getHeight())
	{
		return (false);
	}
	const float IMG1_AVG(getAverage());
	const float IMG2_AVG(anImage.getAverage());
	const float SD(getStandardDeviation() * anImage.getStandardDeviation());
	float ncc_result(0);
	
	for (unsigned int i(0); i < m_width * m_height; ++i)
	{
		ncc_result += ((m_p_image[i] - IMG1_AVG)*(anImage.m_p_image[i] - IMG2_AVG) / SD) / (m_width*m_height);
	}
	
	std::cout << ncc_result << std::endl;
	return ncc_result*100;
}


//----------------------------------------------------------------
float Image::getAverage() const
//----------------------------------------------------------------
{
	float avg(0);

	for (unsigned int i(0); i < m_width*m_height; ++i) 
	{
		avg += m_p_image[i];
	}

	return (avg / (m_width*m_height));
}


//----------------------------------------------------------------
float Image::getVariance() const
//----------------------------------------------------------------
{
	float variance(0);
	float avg(Image::getAverage());

	for (unsigned int i(0); i < m_width*m_height; ++i)
	{
		variance += pow((avg - m_p_image[i]), 2); //squared
	}
	
	return variance /= (m_width*m_height);
}

//----------------------------------------------------------------
float Image::getStandardDeviation() const
//----------------------------------------------------------------
{
	float standard_deviation(sqrt(Image::getVariance()));
	
	return standard_deviation;
}

//----------------------------------------------------------------
Image Image::blendImage(const Image& anImage, float blendRatio) const
//----------------------------------------------------------------
{
	Image tempImage(*this);
	float* p_temp(tempImage.m_p_image);

	if (getWidth() != anImage.getWidth() || anImage.getHeight() != anImage.getHeight())
	{
		std::cout << "Images not of the same size" << std::endl;
	}

	for (unsigned int i(0); i < m_width*m_height; ++i)
	{
		p_temp[i]= (m_p_image[i] + (1-blendRatio) * anImage.m_p_image[i]);
	}
	return tempImage;
}

//----------------------------------------------------------------
float Image::createHistogram(const int aNumberOfBins) const
//----------------------------------------------------------------
{
	std::cout << "bins" << aNumberOfBins << std::endl;
	//create a dynamic array with size bins
	//int bins[bins_to_create] = 0;
	std::vector<unsigned int> bins(aNumberOfBins, 0);
	const float max_value(getMaxValue());
	const float min_value(getMinValue());
	float pixel_range((max_value - min_value)/(float) aNumberOfBins);
	float pixel_floor(0), pixel_ceiling(0);
	int counter(0);

	pixel_floor = min_value;
	pixel_ceiling = min_value + pixel_range;
	for (unsigned int i(0); i < aNumberOfBins; ++i)
	{
		//if(i < range)
		for (int j(0); j < m_width*m_height; ++j)
		{
			if (m_p_image[j] > pixel_floor && m_p_image[j] < pixel_ceiling)
			{
				++counter;
			}
		}
		bins[i] = counter;
		counter = 0;
		pixel_floor = pixel_ceiling;
		pixel_ceiling = pixel_floor + pixel_range;
	}

	for (int i(0); i < aNumberOfBins; ++i) {
		std::cout << i << ": "<< bins[i] << " ," << std::endl;
	}
	//delete[] bins;
	return 0;
}

//----------------------------------------------------------------
unsigned int* Image::getHistogram(unsigned int aNumberOfBins) const
//----------------------------------------------------------------
{
	unsigned int* histogram_data(new unsigned int[aNumberOfBins]);
	std::fill_n(histogram_data, aNumberOfBins, 0);
	const float min_value(getMinValue());
	float range = getMaxValue() - min_value;
	float top;
	int temp;

	for (int i(0); i < m_height*m_width; i++)
	{
		top = m_p_image[i] - min_value;
		temp = aNumberOfBins * (top / range);
		if (temp < aNumberOfBins)
		{
			histogram_data[temp]++;
		} 
		else if(temp == aNumberOfBins)
		{
			histogram_data[temp - 1]++;
		}
	}
	return histogram_data;
}

//----------------------------------------------------------------
void Image::writeHistogram(unsigned int aNumberOfBins, const std::string& aFileName) const
//----------------------------------------------------------------
{
	writeHistogram(aNumberOfBins, aFileName.data());
}
//----------------------------------------------------------------
void Image::writeHistogram(unsigned int aNumberOfBins, const char * aFileName) const
//----------------------------------------------------------------
{
	std::ofstream output_stream(aFileName); // Open the file
	if (!output_stream.is_open()) // The file is not open
	{
		std::string error_message("Cannot open file (output.dat).");
		throw (error_message); // Throw an error
	}

	float range = getMaxValue() - getMinValue();
	float bins = range / aNumberOfBins;
	float minBinValue = getMinValue();
	unsigned int* p_histogram = getHistogram(aNumberOfBins);
	output_stream << "\"Min bin value\"" << " " << "\"Count\"" << std::endl;
	for (int i = 0; i< aNumberOfBins; i++)
	{

		output_stream << minBinValue << " " << p_histogram[i] << std::endl;
		minBinValue += bins;
	}

	output_stream.close();
}



//----------------------------------------------------------------
Image Image::segmentImage(const float threshold) const
//----------------------------------------------------------------
{
	Image tempImage(*this);
	float* p_temp(tempImage.m_p_image);
	const float BLACK = 0;
	const float WHITE = 255;
	
	for (unsigned int i(0); i < m_width*m_height; ++i)
	{
		if (m_p_image[i] < threshold)
		{
			tempImage.m_p_image[i] = BLACK;
		} 
		
		else if (m_p_image[i] > threshold)
		{
			tempImage.m_p_image[i] = WHITE;
		}
		else
		{
			tempImage.m_p_image[i] = m_p_image[i];
		}
	}
	
	return tempImage;
}
//----------------------------------------------------------------
Image Image::selectFunction_3x3(int aFunctionId) const {
	//----------------------------------------------------------------
	Image temp(*this);
	//float* p_temp(temp.m_p_image);//pointer
	const int MIN_WIDTH(m_width - 1), MIN_HEIGHT(m_height - 1), KERNEL_SIZE(9);
	int x(0);
	int y(0);
	float kernel[KERNEL_SIZE];
	//std::cout << "m_width: " << m_width << std::endl;
	//std::cout << "m_height: " << m_height << std::endl;
	while (y++ < m_height - 1) {

		for (x = 0; x < m_width - 1; x++) {
			//block below selects appropriate values and deals with corners
			//regular case
			if (y >= 1 && x >= 1 && y < m_height - 1 && x < m_width - 1)
			{
				kernel[0] = getPixel(x - 1, y - 1); kernel[1] = getPixel(x - 1, y); kernel[2] = getPixel(x - 1, y + 1);
				kernel[3] = getPixel(x, y - 1); kernel[4] = getPixel(x + 1, y - 1); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x + 1, y); kernel[7] = getPixel(x, y + 1); kernel[8] = getPixel(x + 1, y + 1);
			}
			//upper left edge
			else if (y == 0 && x == 0)
			{
				kernel[0] = getPixel(x, y); kernel[1] = getPixel(x, y); kernel[2] = getPixel(x, y);
				kernel[3] = getPixel(x, y); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x + 1, y); kernel[7] = getPixel(x, y + 1); kernel[8] = getPixel(x + 1, y + 1);
			}
			//left edge
			else if (x == 0 && (y != 0 && y != m_height - 1))
			{
				kernel[0] = getPixel(x, y); kernel[1] = getPixel(x, y); kernel[2] = getPixel(x, y);
				kernel[3] = getPixel(x, y - 1); kernel[4] = getPixel(x + 1, y - 1); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x + 1, y); kernel[7] = getPixel(x, y + 1); kernel[8] = getPixel(x + 1, y + 1);
			}
			//bottom right edge
			else if (x == m_width - 1 && y == m_height - 1)
			{
				kernel[0] = getPixel(x - 1, y - 1); kernel[1] = getPixel(x - 1, y); kernel[2] = getPixel(x, y);
				kernel[3] = getPixel(x, y - 1); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x, y); kernel[7] = getPixel(x, y); kernel[8] = getPixel(x, y);
			}
			//bottom left corner
			else if (y == m_height - 1 && x == 0)
			{
				kernel[0] = getPixel(x, y); kernel[1] = getPixel(x, y); kernel[2] = getPixel(x, y);
				kernel[3] = getPixel(x, y); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x, y - 1); kernel[7] = getPixel(x + 1, y - 1); kernel[8] = getPixel(x + 1, y);
			}
			//top right corner
			else if (x == m_width - 1 && y == 0)
			{
				kernel[0] = getPixel(x, y); kernel[1] = getPixel(x, y); kernel[2] = getPixel(x, y);
				kernel[3] = getPixel(x, y); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x - 1, y); kernel[7] = getPixel(x - 1, y + 1); kernel[8] = getPixel(x, y + 1);
			}
			//right side no corners
			else if (x == m_width - 1 && (y != 0 && y != m_height - 1))
			{
				kernel[0] = getPixel(x - 1, y - 1); kernel[1] = getPixel(x - 1, y); kernel[2] = getPixel(x - 1, y + 1);
				kernel[3] = getPixel(x, y - 1); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x, y); kernel[7] = getPixel(x, y + 1); kernel[8] = getPixel(x, y);
			}
			//bottom side
			else if (y == m_height - 1 && (x != 0 && x != m_width - 1))
			{
				kernel[0] = getPixel(x - 1, y - 1); kernel[1] = getPixel(x - 1, y); kernel[2] = getPixel(x + 1, y - 1);
				kernel[3] = getPixel(x, y - 1); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x + 1, y); kernel[7] = getPixel(x, y); kernel[8] = getPixel(x, y);
			}
			//top side
			else if (y == 0 && (x != 0 && x != m_width - 1))
			{
				kernel[0] = getPixel(x, y); kernel[1] = getPixel(x - 1, y); kernel[2] = getPixel(x - 1, y + 1);
				kernel[3] = getPixel(x, y); kernel[4] = getPixel(x, y); kernel[5] = getPixel(x, y);
				kernel[6] = getPixel(x + 1, y); kernel[7] = getPixel(x, y + 1); kernel[8] = getPixel(x + 1, y + 1);
			}
			else { std::cout << "something is amiss here" << std::endl; }

			//get 1D location
			int i = y * m_width + x;
			//storage variables for kernel entries
			float k_val0 = kernel[0], k_val1 = kernel[1], k_val2 = kernel[2], k_val3 = kernel[3], k_val4 = kernel[4], k_val5 = kernel[5],
			k_val6 = kernel[6], k_val7 = kernel[7], k_val8 = kernel[8];
			
			//pixel to be added to temp image
			float aPixelValue(0);
			switch (aFunctionId)//function id selects function
			{
				case(0): aPixelValue = temp.filter_Median(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;	//median filter
				case(1): aPixelValue = temp.filter_Laplacian(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break; //laplacian edge detection
				case(2): aPixelValue = temp.blur_Gaussian(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;	//Gaussian blur
				case(3): aPixelValue = temp.blur_Box(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;		//box blur
				case(4): aPixelValue = temp.sharpenImg(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;		//sharpen
				case(5): aPixelValue = temp.filter_Prewitt(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;	//prewitt operator
				case(6): aPixelValue = temp.filter_Sobel(k_val0, k_val1, k_val2, k_val3, k_val4, k_val5, k_val6, k_val7, k_val8); break;	//sobel operator
				default: std::cout << "Method ID not found" << std::endl; break;
				//end program
			}
			//add new pixel value to temp image
			temp.m_p_image[i] = aPixelValue;
		} // end of x iterator
	} // end of y iterator
	//return temp image when done
	return (temp);
}

//----------------------------------------------------------------
float Image::filter_Median(float n0, float n1, float n2, float n3, 
	float n4, float n5, float n6, float n7, float n8)
//----------------------------------------------------------------
{
	int median(0);
	//kernel: original values x 1
	float kernel[9] = { n0,n1,n2,n3,n4,n5,n6,n7,n8 };
	//sort first to last
	std::sort(kernel, kernel + 9);
	//kernel median value
	median = kernel[4];
	//return median filter
	return median;
}

//rename to laplacian
//----------------------------------------------------------------
float Image::filter_Laplacian(float n0, float n1, float n2, float n3,
	float n4, float n5, float n6, float n7, float n8)
//----------------------------------------------------------------
{
	//inage data in array
	float size(9);
	//image data
	float imageData[9] = { n0, n1, n2, n3, n4, n5, n6, n7, n8 };
	//kernel for edge detection laplacian
	float kernel[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0};
	int aPixelValue(0);
	float sum1, sum2;

	//multiplies each kernel enty with each pixel
	for (int i(0); i < size; i++) {
		imageData[i] *= kernel[i];
		//sums values
		aPixelValue += imageData[i];
	}
	//returns sum
	return aPixelValue;
}

//----------------------------------------------------------------
int Image::filter_Sobel(float n0, float n1, float n2, float n3,
	float n4, float n5, float n6, float n7, float n8)
//----------------------------------------------------------------
{	
	//sobel kernel for x axis
	float kernel_X[9] = {1, 2, 1, 0 ,0 , 0, -1, -2, -1};
	//sobel kernel for y axis
	float kernel_Y[9] = {1, 0, -1, 2 , 0 , -2, 1, 0, -1};
	//original image data
	float imageData[9] = {n1,n2,n3,n4,n5,n6,n7,n8,};
	//sum of first kernel
	float kernelSum_1(0);
	float kernelSum_2(0);
	float aPixelValue(0);
	float normalize(0);


	for (int i(0); i < 9; i++)
	{
		kernelSum_1 += imageData[i] * kernel_X[i];
		kernelSum_2 += imageData[i] * kernel_Y[i];
	}
	//return new pixel value
	aPixelValue = abs(kernelSum_1) + abs(kernelSum_2);
	//return new pixel value
	return aPixelValue;
}

//----------------------------------------------------------------
float Image::filter_Prewitt(float n0, float n1, float n2, float n3,
	float n4, float n5, float n6, float n7, float n8)
//----------------------------------------------------------------
{
	//image data
	float imageData[9] = { n0, n1, n2, n3, n4, n5, n6, n7, n8 };
	//prewitt kernel for x axis
	float kernel_X[9] = { 1, 0, -1,  1,  0,  -1,  1,  0, -1 };
	//prewitt kernel for y axis
	float kernel_Y[9] = {1,  1,  1,  0,  0,  0,-  1, -1, -1};
	float kernelSum_1(0), kernelSum_2(0), aPixelValue(0);
	//get kernel sums and multiply data with kernel
	for(int i(0); i < 9; i++)
	{
		kernelSum_1 += imageData[i] * kernel_X[i];
		kernelSum_2 += imageData[i] * kernel_Y[i];
	}
	//get absolute sum of both
	aPixelValue = abs(kernelSum_1) + abs(kernelSum_2);
	//return new pixel value
	return aPixelValue;
}
//----------------------------------------------------------------
float Image::blur_Box(float n0, float n1, float n2, float n3, float n4,
	float n5, float n6, float n7, float n8) //vector
//----------------------------------------------------------------
{
	float size(9);
	float imageData[9] = { n0, n1, n2, n3, n4, n5, n6, n7, n8 };
	float aPixelValue(0);
	//get get total sum of all kernel data
	for (int i(0); i < size; i++) {
		aPixelValue += imageData[i];
	}
	//normalize
	aPixelValue /= size;
	//return new pixel value
	return aPixelValue;
}
//----------------------------------------------------------------
float Image::blur_Gaussian(float n0, float n1, float n2, float n3, 
	float n4, float n5, float n6, float n7, float n8)
//----------------------------------------------------------------
{
	float size(9);
	//original data
	float imageData[9] = { n0, n1, n2, n3, n4, n5, n6, n7, n8 };
	//gaussian kernel
	float kernel[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	float aPixelValue(0);
	//multiply original data with kernel and sum
	for (int i(0); i < size; i++) 
	{
		imageData[i] *= kernel[i];
		aPixelValue += imageData[i];
	}
	//normalize
	aPixelValue /= 16;
	//return new pixel value
	return aPixelValue;
}
//----------------------------------------------------------------
float Image::sharpenImg(float n0, float n1, float n2, float n3, 
	float n4, float n5, float n6, float n7, float n8) // alpha
//----------------------------------------------------------------
{
	float size(9);
	//orginal data
	float imageData[9] = { n0, n1, n2, n3, n4, n5, n6, n7, n8 };
	//sharpen kernel
	float kernel[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	float aPixelValue(0);
	//multiply each pixel in original data with kernel and compute sum
	for (int i(0); i < 9; i++) {
		imageData[i] *= kernel[i];
		aPixelValue += imageData[i];
	}
	//return new value
	return aPixelValue;
}