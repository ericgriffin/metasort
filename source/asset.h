/*  asset.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _ASSET_H_
#define _ASSET_H_ 1

class asset
    {
    public:
		asset();
		~asset();
		char path[255];
		char filename[255];
		char full_filename[255];
		char extension[255];

	private:
		
	};
	
#endif // _ASSET_H_