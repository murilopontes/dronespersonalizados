/*
 * (c) Seva Petrov 2002. All Rights Reserved.
 *
 * --LICENSE NOTICE--
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * --LICENSE NOTICE--
 *
 * $Date: 2003/05/30 06:07:05 $
 * $Id: TestClient.cs,v 1.1 2003/05/30 06:07:05 metaforge Exp $
 * 
 */
using System;
using System.IO;
using De.Mud.Telnet;

namespace Net.Graphite.Telnet
{
	/// <summary>
	/// A simple console testbed for the Telnet application.
	/// </summary>
	public class TestClient
	{
		[STAThread]
		static void Main(string[] args)
		{
			if (args.Length < 3 || (args[0] != "i" && args[0] != "s"))
			{
				Console.WriteLine("Usage: TestClient [mode] [host] [port]");
				Console.WriteLine("mode: [i]nteractive/[s]cripted");
				return;
			}

			//string[] args = new string[3];
			//args[0] = "i";
			//args[1] = "w2ks1";
			//args[2] = "23";

			TestRun tr = new TestRun(args[1], Int32.Parse(args[2]));

			try 
			{
				if (args[0] == "i")
					tr.InteractiveSession();// edit the method for specific cmds
				else
					tr.ScriptedSession();
			}
			catch (Exception e) 
			{
				Console.WriteLine("An exception has occurred: " + 
					e.Message + "\n\n" + e.StackTrace + "\n");
			} 
		}
	}

	public class TestRun
	{
		private TelnetWrapper t; 
		private bool done = false;
		private StreamReader sr;

		public TestRun(string host, int port) 
		{
			t = new TelnetWrapper();
			
			t.Disconnected  += new DisconnectedEventHandler(this.OnDisconnect);
			t.DataAvailable += new DataAvailableEventHandler(this.OnDataAvailable);
			
			t.TerminalType = "NETWORK-VIRTUAL-TERMINAL";
			t.Hostname = host;
			t.Port = port;
			Console.WriteLine("Connecting ...");
			t.Connect();
		}

		public void ScriptedSession()
		{
			Console.WriteLine("Not implemented.");
		}

		public void InteractiveSession()
		{
			int i;
			char ch;
		
			try 
			{
				t.Receive();
			
				while (!done)
					t.Send(Console.ReadLine() + t.CRLF);
			}
			catch 
			{
				t.Disconnect();
				throw;
			}
		}

		private void OnDisconnect(object sender, EventArgs e)
		{
			done = true;
			Console.WriteLine("\nDisconnected.");
		}

		private void OnDataAvailable(object sender, DataAvailableEventArgs e)
		{
			Console.Write(e.Data);
		}
	}
}
