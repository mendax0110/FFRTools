#include <boost/lexical_cast.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
//#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "Colourisers.h"
#include "Calculators.h"

int main(int argc, const char** argv)
{
	if(6 != argc)
	{
		std::cout	<< "Params\n"
					<< "\t<number of z axis slices>\n"
					<< "\t<number of xy slices (pixel size)>\n"
					<< "\t<axis size in mm>\n"
					<< "\t<radius of poissor in mm>\n"
					<< "\t<input voltage (kV)>\n";
		return 0;
	}
	
	int c1, c2, c3, c4, c5;
	
	
	try
	{
		c1 = boost::lexical_cast<int>(argv[1]);
		c2 = boost::lexical_cast<int>(argv[2]);
		c3 = boost::lexical_cast<int>(argv[3]);
		c4 = boost::lexical_cast<int>(argv[4]);
		c5 = boost::lexical_cast<int>(argv[5]) * 1000;
	}
	catch( const boost::bad_lexical_cast& ex)
	{
		std::cout << "Unable to understand parameters. Use integer values only!\n" << ex.what();
		return 0;
	}
	
	const int z_slices = c1;
	const int xy_slices = c2;
	const int axis_max = c3;
	const int radius = c4;
	const int kV = c5;
	
	// Find out what each increment in position means (accounting for measurements in mm and not metres).
	const double z_space =
		(static_cast< double >(axis_max) / static_cast< double >(z_slices)) / 1000;
	const double xy_space = 
		(static_cast< double >(axis_max) / static_cast< double >(xy_slices)) / 1000;
		
	const int xy_half = xy_slices / 2;
	
	double minPotential = std::numeric_limits< double >::max();
	double maxPotential = std::numeric_limits< double >::min();
	
	double z_pos = 0.0;
	for( int z = (z_slices / 2); z < z_slices; z++, z_pos+=z_space)
	{
		double x_pos = 0.0;
		
		const int top = z_slices - z;
		const int bottom = z;
		
		std::cout << "Calculating slice " << (z - (z_slices / 2)) + 1 << " of " << z_slices / 2 << std::endl;
		
		std::string topName = "slice";
		std::string bottomName = "slice";
		
		try
		{
			topName += boost::lexical_cast<std::string>(top);
			bottomName += boost::lexical_cast<std::string>(z_slices - top);
		}
		catch( const boost::bad_lexical_cast& )
		{
			std::cout	<< "Unable to create name of file for slices << "
						<< top << " and " << bottom << std::endl;
		}
		
		topName += ".jpg";
		bottomName += ".jpg";
		
		boost::gil::rgb8_image_t img(xy_slices, xy_slices);
		boost::gil::rgb8_view_t vw = boost::gil::view(img);
			
		for(int x = 0; x < xy_half + (xy_half % 2); x++, x_pos+=xy_space)
		{
			double y_pos = 0.0;
			
			for(int y = 0; y < xy_half + (xy_half % 2); y++, y_pos+=xy_space)
			{				
				const double potential = calcPotentialAtPoint(
					x_pos, y_pos, z_pos, radius, kV);
				minPotential = std::min(minPotential, potential);
				maxPotential = std::max(maxPotential, potential);
				
				int8_t r = 0;
				int8_t g = 0;
				int8_t b = 0;
				
				colourise(potential, kV, r, g, b);
				
				vw(xy_half + x, xy_half + y) = boost::gil::rgb8_pixel_t(r, g, b);
				vw(xy_half + x, xy_half - y) = boost::gil::rgb8_pixel_t(r, g, b);
				vw(xy_half - x, xy_half + y) = boost::gil::rgb8_pixel_t(r, g, b);
				vw(xy_half - x, xy_half - y) = boost::gil::rgb8_pixel_t(r, g, b);
				
				y_pos += xy_space;
			}
			
			x_pos += xy_space;
		}
		
		/*boost::gil::write_view(topName, vw);
		if(top != bottom)
		{
			boost::gil::write_view(bottomName, vw);
		}*/
		
		z_pos += z_space;
		
		for(int x = 0; x < xy_slices; x++)
		{
			for(int y = 0; y < xy_slices; y++)
			{
			}
		}
	}
	
	std::cout << "Min = " << minPotential << "\n";
	std::cout << "Max = " << maxPotential << "\n";

	return 1;
}
