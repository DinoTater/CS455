using System; // For String, Int32, Console, ArgumentException
1 using System.Text; // For Encoding
2 using System.IO; // For IOException
3 using System.Net.Sockets; // For TcpClient, NetworkStream, SocketException
4
5 class TcpEchoClient {
6
18 Chapter 2: Basic Sockets ■
7 static void Main(string[] args) {
8
9 if ((args.Length < 2) || (args.Length > 3)) { // Test for correct # of args
10 throw new ArgumentException("Parameters: <Server> <Word> [<Port>]");
11 }
12
13 String server = args[0]; // Server name or IP address
14
15 // Convert input String to bytes
16 byte[] byteBuffer = Encoding.ASCII.GetBytes(args[1]);
17
18 // Use port argument if supplied, otherwise default to 7
19 int servPort = (args.Length == 3) ? Int32.Parse(args[2]) : 7;
20
21 TcpClient client = null;
22 NetworkStream netStream = null;
23
24 try {
25 // Create socket that is connected to server on specified port
26 client = new TcpClient(server, servPort);
27
28 Console.WriteLine("Connected to server... sending echo string");
29
30 netStream = client.GetStream();
31
32 // Send the encoded string to the server
33 netStream.Write(byteBuffer, 0, byteBuffer.Length);
34
35 Console.WriteLine("Sent {0} bytes to server...", byteBuffer.Length);
36
37 int totalBytesRcvd = 0; // Total bytes received so far
38 int bytesRcvd = 0; // Bytes received in last read
39
40 // Receive the same string back from the server
41 while (totalBytesRcvd < byteBuffer.Length) {
42 if ((bytesRcvd = netStream.Read(byteBuffer, totalBytesRcvd,
43 byteBuffer.Length - totalBytesRcvd)) == 0) {
44 Console.WriteLine("Connection closed prematurely.");
45 break;
46 }
47 totalBytesRcvd += bytesRcvd;
48 }
49
■ 2.3 TCP Sockets 19
50 Console.WriteLine("Received {0} bytes from server: {1}", totalBytesRcvd,
51 Encoding.ASCII.GetString(byteBuffer, 0, totalBytesRcvd));
52
53 } catch (Exception e) {
54 Console.WriteLine(e.Message);
55 } finally {
56 netStream.Close();
57 client.Close();
58 }
59 }
60 }