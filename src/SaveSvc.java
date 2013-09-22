/*
	JBit - A 6502 framework for mobile phones
	Copyright (C) 2007-2010  Emanuele Fornara
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * Reserved opId range: 6xx
 */
public interface SaveSvc {
	
	String TAG = "Save";
	
	int RETCODE_INVPAR = -1;
	int RETCODE_EXECUTED = 1;
	int RETCODE_PENDING = 2;

	/**
	 * <b>retCode opI(0, new Object[] {
	 *     java.lang.Boolean newName,
	 *     Displayable nextDisplayable,
	 *     CommandListener listener,
	 *     Command okCommand,
	 *     Command failedCommand
	 * })</b>
	 * 
	 * <p>Save the current program and notify the listener when
	 * the operation completes (note: notification may be carried out
	 * in the opI call, so the caller should be coded to be
	 * reentrant). Listener is called with a null Displayable.
	 * 
	 * <p>Unless retCode is RETCODE_INVPAR (a bug), the service is
	 * in charge of notifying the user of the outcome of the operation.
	 */
	int OP_SAVE = 601;

}
