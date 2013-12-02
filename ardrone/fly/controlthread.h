/*
    controlthread.h - AR.Drone control thread

    Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/
#ifndef _CONTROLTRHEAD_H
#define _CONTROLTRHEAD_H
void ctl_Init();
int ctl_FlatTrim();
void ctl_SetSetpoint(float pitch, float roll, float yawsp, float h);
void ctl_SetSetpointDiff(float pitch, float roll, float yaw, float h);
void ctl_Close();
void ctl_SetGas(float gas);
#endif
