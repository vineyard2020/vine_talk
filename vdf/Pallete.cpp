/*
 * Copyright 2018 Foundation for Research and Technology - Hellas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0 [1] [1]
 *
 * Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 *  implied.
 * See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * Links:
 *  ------
 * [1] http://www.apache.org/licenses/LICENSE-2.0 [1]
*/


#include "Pallete.h"
#include <sstream>
#include <random>
#include <algorithm>
#include <iostream>

std::vector<std::string> Pallete :: colors;
std::set<size_t> Pallete :: unused_colors;
std::map<std::string,size_t> Pallete :: named_colors;


void Pallete :: init()
{
	if(!colors.size())
	{
		for(int r = 0 ; r < 16 ; r++)
			for(int g = 0 ; g < 16 ; g++)
				for(int b = 0 ; b < 16 ; b++)
				{
					std::string c = "";
					if(r < 10)
						c += '0'+r;
					else
						c += 'A'+(r-10);
					if(g < 10)
						c += '0'+g;
					else
						c += 'A'+(g-10);
					if(b < 10)
						c += '0'+b;
					else
						c += 'A'+(b-10);
					colors.push_back(c);
				}
		std::shuffle(colors.begin(),colors.end(),std::default_random_engine(0));
		for(int cnt = 0 ; cnt < colors.size() ; cnt++)
			unused_colors.insert(cnt);
	}
}

std::string Pallete :: get(size_t id,int opacity)
{
	static const char hex[] = "0123456789ABCDEF";
	if(opacity > 15)
		opacity = 15;
	if(opacity < 0)
		opacity = 0;
	std::ostringstream oss;

	init();

	unused_colors.erase(id);
	oss << colors[id] << hex[opacity];

	return oss.str();
}

std::string Pallete :: get(std::string name,int opacity)
{

	init();

	if(named_colors.count(name) == 0)
	{
		size_t new_color = *unused_colors.begin();
		unused_colors.erase(new_color);
		named_colors[name] = new_color;
	}
	return get(named_colors[name],opacity);
}
