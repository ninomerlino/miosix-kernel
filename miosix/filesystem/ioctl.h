/***************************************************************************
 *   Copyright (C) 2013 by Terraneo Federico                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef IOCTL_H
#define	IOCTL_H

namespace miosix {

enum Ioctl
{
    IOCTL_SYNC=100,
    IOCTL_TCGETATTR=101,
    IOCTL_TCSETATTR_NOW=102,
    IOCTL_TCSETATTR_FLUSH=103,
    IOCTL_TCSETATTR_DRAIN=104,
    IOCTL_FLUSH=105,
    IOCTL_GET_BLOCK_SIZE = 106, // Get the block size of the device in Bytes
    IOCTL_GET_SECTOR_SIZE = 107, // Get the sector size of the device in KBytes
    IOCTL_GET_SECTOR_COUNT = 108, // Get the number of sectors of the device
    IOCTL_GET_DEVICE_SIZE = 109 // Get the size of the device in KBytes
};

}

#endif //IOCTL_H
