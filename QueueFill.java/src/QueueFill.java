import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import gnu.io.*;
import lg.util.dbg;

public class QueueFill
{
    QueueFill(String portName) throws Exception
    {
        SerialPort serialPort;
        OutputStream outStream;
        InputStream inStream;
        String str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        byte[] strData = str.getBytes();
        dbg.println(1, "strData len="+strData.length);
        try
        {
            CommPortIdentifier portId =
                    CommPortIdentifier.getPortIdentifier(portName);
          serialPort = (SerialPort) portId.open("ESP8266", 5000);
          int baudRate = 115200;
          // Set serial port to 115200bps-8N1
          serialPort.setSerialPortParams(
              baudRate,
              SerialPort.DATABITS_8,
              SerialPort.STOPBITS_1,
              SerialPort.PARITY_NONE);
          dbg.println(19, "Before setFlowControlMode");
          serialPort.setFlowControlMode(
                      SerialPort.FLOWCONTROL_NONE);
          dbg.println(19, "After setFlowControlMode");
          outStream = serialPort.getOutputStream();
          inStream = serialPort.getInputStream();
          int idx = 0;
          byte[] data = new byte[128];
          while(true)
          {
              dbg.println(11, "Writing data "+ idx);
              idx++;
              outStream.write(strData);
              Thread.sleep(10);
              int num = inStream.read(data);
              dbg.println(11, "Data is read "+ num);
          }
        }catch (NoSuchPortException e)
        {
            throw new Exception("ESP8266_io - NoSuchPortException portName=" + portName);
        }catch (PortInUseException e)
        {
            throw new Exception("ESP8266_io - PortInUseException portName=" + portName);
        }catch (UnsupportedCommOperationException e)
        {
            throw new Exception("ESP8266_io - UnsupportedCommOperationException portName=" + portName);
        }catch (IOException e)
        {
            throw new Exception("ESP8266_io - IOException portName=" + portName);
        }
    }
    public static void main(String[] args)
    {
        dbg.set(99);
        try
        {
            String portName;
            if (args.length == 0)
                portName = "COM5";
            else
                portName = args[0]; 
            new QueueFill(portName);
        }catch (Exception e)
        {
            System.err.println("main.Exception e="+e.toString());
        }
    }
}
