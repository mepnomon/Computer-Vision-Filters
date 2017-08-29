/**
********************************************************************************
*
*	@file		test2.cpp
*
*	@brief		Tests for: getSAE, getNCC, image blending,threshold segmentation,
*				median filter, sobel filter, prewitt filter, laplacian filter,
*				box blur.
*
*	@version	1.0
*
*	@date		23/12/2016
*
*	@author		Dorian Dressler
*
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <sstream>
#include <iostream>
#include <exception>

#include "Image.h"


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    // Return code
    int error_code(0);
    
    // Catch exceptions
    try
    {
			// Load an image
		Image input_1, input_2, input_3, input_4;
		input_1.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\enterprise.pgm");
		input_2.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\brian_kernighan.pgm");
		input_3.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\lena512.pgm");
		input_4.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\ent_noise.pgm");

		//load reference images
		Image ref_1, ref_2, ref_3, ref_4, ref_5, ref_6, ref_7, ref_8;
		ref_1.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\medianFilter.pgm");
		ref_2.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\laplacian.pgm");
		ref_3.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\gaussianBlur.pgm");
		ref_4.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\boxblur.pgm");
		ref_5.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\sharperImg.pgm");
		ref_6.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\segmented.pgm");
		ref_7.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\prewitt.pgm");
		ref_8.loadPGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\Reference\\sobel.pgm");
		
		//test output
		Image medianFilteredImage; 
		medianFilteredImage = input_4.selectFunction_3x3(0);
		medianFilteredImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\median_filter_out.pgm");
		medianFilteredImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\median_filter_out.ascii");
		medianFilteredImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\median_filter_out.raw");
		std::cout << (std::abs(medianFilteredImage.getNCC(ref_1) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(medianFilteredImage.getSAE(ref_1)) << std::endl;

		Image laplacianFilterImage;
		laplacianFilterImage = input_1.selectFunction_3x3(1);
		laplacianFilterImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\laplacian_edgedetection_out.pgm");
		laplacianFilterImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\laplacian_degedetection_out.ascii");
		laplacianFilterImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\laplacian_edgedetection_out.raw");
		std::cout << (std::abs(laplacianFilterImage.getNCC(ref_2) - 1.0) < 1.0e-3? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(laplacianFilterImage.getSAE(ref_2)) << std::endl;

		Image gaussianBlurImage;
		gaussianBlurImage = input_4.selectFunction_3x3(2);
		gaussianBlurImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\gaussian_blur_out.pgm");
		gaussianBlurImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\gaussian_blur_out.ascii");
		gaussianBlurImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\gaussian_blur_out.raw");

		gaussianBlurImage.getSAE(input_4);
		std::cout << (std::abs(gaussianBlurImage.getNCC(ref_3) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(gaussianBlurImage.getSAE(ref_3)) << std::endl;
		
		Image boxBlurImage;
		boxBlurImage = input_4.selectFunction_3x3(3);
		boxBlurImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\box_blur_out.pgm");
		boxBlurImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\box_blur_out.ascii");
		boxBlurImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\box_blur_out.raw");
		std::cout << (std::abs(boxBlurImage.getNCC(ref_4) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(boxBlurImage.getSAE(ref_4)) << std::endl;

		Image sharperImage;
		sharperImage = input_1.selectFunction_3x3(4);
		sharperImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sharpened_out.pgm");
		sharperImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sharpened_out.ascii");
		sharperImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sharpened_out.raw");
		std::cout << (std::abs(sharperImage.getNCC(ref_5) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(sharperImage.getSAE(ref_5)) << std::endl;

		Image segmentedImage;
		segmentedImage = input_1.segmentImage(125);
		segmentedImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\segmented_out.pgm");
		segmentedImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\segmented_out.ascii");
		segmentedImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\segmented_out.raw");
		std::cout << (std::abs(segmentedImage.getNCC(ref_6) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(segmentedImage.getSAE(ref_6)) << std::endl;

		Image blendedImage; //x
		blendedImage = input_2.blendImage(input_3, 0.5);
		blendedImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\blended_out.pgm");
		blendedImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\blended_out.ascii");
		blendedImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\blended_out.raw");;

		Image prewittImage; //x
		prewittImage = input_1.selectFunction_3x3(5);
		prewittImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\prewitt_edgedetection_out.pgm");
		prewittImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\prewitt_edgedetection_out.ascii");
		prewittImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\prewitt_edgedetection_out.raw");
		std::cout << (std::abs(prewittImage.getNCC(ref_7) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl;
		std::cout << abs(prewittImage.getSAE(ref_7)) << std::endl;

		Image sobelImage; //x
		sobelImage = input_1.selectFunction_3x3(6);
		sobelImage.savePGM("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sobel_edgedetection_out.pgm");
		sobelImage.saveASCII("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sobel_edgedetection_out.ascii");
		sobelImage.saveRaw("C:\\Programs\\C++\\computer vision\\Assignments\\CV Assignment 2\\test_data\\results\\sobel_edgedetection_out.raw");
		//sobelImage.getNCC(input_1);
		std::cout << (std::abs(sobelImage.getNCC(ref_8) - 1.0) < 1.0e-3 ? "SUCCESS" : "FAILURE") << std::endl; 
		std::cout << abs(sobelImage.getSAE(ref_8)) << std::endl;
    }
    // An error occured
    catch (const std::exception& error)
    {
        error_code = 1;
        std::cerr << error.what() << std::endl;
    }
    catch (const std::string& error)
    {
        error_code = 1;
        std::cerr << error << std::endl;
    }
    catch (const char* error)
    {
        error_code = 1;
        std::cerr << error << std::endl;
    }
    catch (...)
    {
        error_code = 1;
        std::cerr << "Unknown error" << std::endl;
    }

    return (error_code);
	
}
