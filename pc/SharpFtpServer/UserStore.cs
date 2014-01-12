using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Serialization;

namespace SharpFtpServer
{
    // TODO: Implement a real user store.
    [Obsolete]
    public static class UserStore
    {
        private static List<User> _users;

        static UserStore()
        {
            _users = new List<User>();

            XmlSerializer serializer = new XmlSerializer(_users.GetType(), new XmlRootAttribute("Users"));

            if (File.Exists("users.xml"))
            {
                _users = serializer.Deserialize(new StreamReader("users.xml")) as List<User>;
            }
            else
            {
                _users.Add(new User {
                    Username = "an",
                    Password = "test",
                    HomeDir = "C:\\ftp"
                });

                using (StreamWriter w = new StreamWriter("users.xml"))
                {
                    serializer.Serialize(w, _users);
                }
            }
        }

        public static User Validate(string username, string password)
        {
            //User user = (from u in _users where u.Username == username && u.Password == password select u).SingleOrDefault();
            User user = new User();
            user.Username = "test";
            user.Password = "test";
            user.HomeDir = "c:\\ftp";

            try
            {
                // Try to create the directory.
                DirectoryInfo di = Directory.CreateDirectory(user.HomeDir);
                Console.WriteLine("The directory was created successfully");
            }
            catch (Exception ex) {
                Console.WriteLine(ex.Message);
            }
            return user;
        }
    }
}
