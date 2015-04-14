﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Globalization;

namespace COMParse
{
    class Program
    {
        enum COM_SOURCE
        {
            HOST = 0,
            EMOTE = 1
        };

        enum PayloadOperation
        {
            RAW = 0,
            CHAR = 1,
            SUPPRESS
        };

        static int getPayloadSize(String rawString){
            int payloadSize = 0;

            // four byte payload
            string[] parsedRaw = rawString.Split(' ');
            //payloadSize = int.Parse(parsedRaw[3], NumberStyles.HexNumber) + int.Parse(parsedRaw[2], NumberStyles.HexNumber) + int.Parse(parsedRaw[1], NumberStyles.HexNumber) + int.Parse(parsedRaw[0], NumberStyles.HexNumber);
            payloadSize = int.Parse(parsedRaw[1], NumberStyles.HexNumber) * 0x100 + int.Parse(parsedRaw[0], NumberStyles.HexNumber);
            return payloadSize;
        }

        static bool parseHex(String input, ref String evaluatedOperation, COM_SOURCE source, ref String payload, ref PayloadOperation payloadOp)
        {
            String parsedInput = input;
            evaluatedOperation = "";
            bool waitForParameter = false;
            try
            {
                parsedInput = input.Substring(16 * 3, 11);
                parsedInput = parsedInput.ToUpper();
                payload = "";
                payloadOp = PayloadOperation.RAW;
                if (parsedInput.Equals("00 00 00 00") == true)
                {
                    evaluatedOperation = "PING";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("01 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Message";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Message ACK";
                        waitForParameter = false;
                    }
                    payloadOp = PayloadOperation.CHAR;
                }                
                else if (parsedInput.Equals("02 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Read memory:";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Read memory ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("03 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        int payloadSize = 0;
                        if (input.Length > (28 * 3))
                        {
                            payloadSize = getPayloadSize(input.Substring(28 * 3, 11));
                        }
                        evaluatedOperation = "Write memory " + payloadSize.ToString() + " bytes <data suppressed>";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Write memory ACK";
                        waitForParameter = false;
                    }
                    payloadOp = PayloadOperation.SUPPRESS;
                }
                else if (parsedInput.Equals("04 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Check memory:";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Check memory ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("05 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Erase sector:";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Erase sector ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("06 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                        evaluatedOperation = "EXECUTE";
                    else
                    {
                        evaluatedOperation = "EXECUTE ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("07 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                        evaluatedOperation = "REBOOT";
                    else
                    {
                        evaluatedOperation = "REBOOT ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("08 00 00 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Memory map";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Memory map ACK";
                        waitForParameter = false;
                    }
                }                
                else if (parsedInput.Equals("09 00 00 00") == true)
                {
                    evaluatedOperation = "EXIT";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("0A 00 00 00") == true)
                {
                    evaluatedOperation = "CHECK SIGNATURE";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("0B 00 00 00") == true)
                {
                    evaluatedOperation = "Deployment map";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("0C 00 00 00") == true)
                {
                    evaluatedOperation = "Flash sector map";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("0D 00 00 00") == true)
                {
                    evaluatedOperation = "Signature key update";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("0E 00 00 00") == true)
                {
                    evaluatedOperation = "OEM information";
                    waitForParameter = false;
                }
                else if (parsedInput.Equals("01 00 02 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Reset debugger state";
                        waitForParameter = true;
                    }
                    else
                    {
                        evaluatedOperation = "Reset debugger state ACK";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("03 00 02 00") == true)
                {
                    evaluatedOperation = "Unlock low-level command";
                    waitForParameter = false;
                    payloadOp = PayloadOperation.CHAR;
                }
                else if (parsedInput.Equals("08 00 02 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        waitForParameter = true;
                        evaluatedOperation = "CLR Capability request";
                    }
                    else
                    {
                        evaluatedOperation = "CLR Capability request ACK";
                        waitForParameter = false;
                        payloadOp = PayloadOperation.CHAR;
                    }
                }
                else if (parsedInput.Equals("40 00 02 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "List all assemblies";
                        waitForParameter = false;
                    }
                    else
                    {
                        evaluatedOperation = "List all assemblies";
                        waitForParameter = false;
                    }
                }
                else if (parsedInput.Equals("50 00 02 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Resolves an assembly";
                        waitForParameter = false;
                    }
                    else
                    {
                        evaluatedOperation = "Resolves an assembly";
                        waitForParameter = false;
                        payloadOp = PayloadOperation.CHAR;
                    }
                }
                else if (parsedInput.Equals("B0 00 02 00") == true)
                {
                    if (source == COM_SOURCE.HOST)
                    {
                        evaluatedOperation = "Return entryPoint";
                        waitForParameter = false;
                    }
                    else
                    {
                        evaluatedOperation = "Return entryPoint";
                        waitForParameter = false;
                    }
                }
                else
                {
                    evaluatedOperation = parsedInput;
                    waitForParameter = false;
                }                
            }
            catch
            {
                evaluatedOperation = "Unknown";
                waitForParameter = false;
            }

            try
            {
                if (input.Length > (28 * 3))
                {
                    int payloadSize = getPayloadSize(input.Substring(28 * 3, 11));
                    if (payloadSize > 0)
                    {
                        if (input.Length > (32 * 3))
                            payload = input.Substring(32 * 3, (payloadSize * 3) - 1);
                        else
                        {
                            payload = "";
                            waitForParameter = true;
                        }
                    }
                    else
                        payload = "";
                }
                else
                    payload = "";
            }
            catch
            {
                payload = "";
                waitForParameter = true;
            }

            return waitForParameter;
        }

        static void Main(string[] args)
        {
            try
            {
                StreamReader sr;
                String line;
                StreamWriter sw;
                String outString = "";
                bool printOutString = false;
                bool parseAfterSuccess = false;
                bool parseEverything = false;
                bool ignoreToBreak = false;
                bool waitForParameter = false;
                String fullCommandString = "";
                String parsedCommand = "";
                String keepCommandString = "";
                COM_SOURCE lastMessageSource = COM_SOURCE.HOST;
                String payload = "";
                PayloadOperation payloadOp = PayloadOperation.RAW;
                //String baseName = "empty project - VS - plug in quick deploy";
                //String baseName = "4-16-13 build - successful clean VS deploy of hello world";
                String baseName = "deployToEmpty";
                //String baseName = "fullDeploy";
                //String baseName = "empty project -VS -plug in wait 10 seconds to deploy";
                String fileName = baseName + ".txt";
                String outFileName = "out_" + fileName;
                sr = new StreamReader(fileName);
                sw = new StreamWriter(outFileName);
                line = sr.ReadLine();
                while (line != null)
                {
                    if (line.StartsWith("------") == true)
                    {
                        //System.Diagnostics.Debug.WriteLine("Break");
                        if (fullCommandString != "")
                        {
                            if (waitForParameter == false)
                            {
                                // here there is no command string kept from earlier so we read normally
                                waitForParameter = parseHex(fullCommandString, ref parsedCommand, lastMessageSource, ref payload, ref payloadOp);
                                if (payloadOp == PayloadOperation.RAW)
                                {
                                    if (lastMessageSource == COM_SOURCE.HOST)
                                        outString += parsedCommand + " " + payload;
                                    else
                                        outString += "\t\t\t\t\t\t\t\t\t\t\t\t\t" + parsedCommand + " " + payload;
                                    if (parsedCommand.Equals("Unknown") == true)
                                        outString += "\t\t" + fullCommandString;
                                }
                                else if (payloadOp == PayloadOperation.SUPPRESS)
                                {
                                    if (lastMessageSource == COM_SOURCE.HOST)
                                        outString += parsedCommand;
                                    else
                                        outString += "\t\t\t\t\t\t\t\t\t\t\t\t\t" + parsedCommand;
                                    if (parsedCommand.Equals("Unknown") == true)
                                        outString += "\t\t" + fullCommandString;
                                }
                                else
                                {
                                    if (lastMessageSource == COM_SOURCE.HOST)
                                        outString += parsedCommand + " ";
                                    else
                                        outString += "\t\t\t\t\t\t\t\t\t\t\t\t\t" + parsedCommand + " ";

                                    while (payload.Length >= 3)
                                    {
                                        try
                                        {
                                            String temp = payload.Substring(0, 2);
                                            int tempInt = (int.Parse(temp, NumberStyles.HexNumber));
                                            char tempChar = (char)tempInt;
                                            if (tempInt >= 32 && tempInt <= 126)
                                                outString += tempChar.ToString();
                                            else
                                                outString += temp + " ";
                                            payload = payload.Substring(3, payload.Length - 3);
                                        }
                                        catch
                                        {
                                            System.Diagnostics.Debug.WriteLine("char parse error");
                                        }
                                    }

                                    if (parsedCommand.Equals("Unknown") == true)
                                        outString += "\t\t" + fullCommandString;
                                }
                                /*if (lastMessageSource == COM_SOURCE.HOST)
                                    outString += "\r\n\t\t\t\t\t\t\t\t\t\t\t" + fullCommandString;
                                else
                                    outString += "\r\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + fullCommandString;*/
                                keepCommandString = outString;
                            }
                            else
                            {
                                // here we read a command string previously so now we need to read in the parameter
                                //parseParameter(fullCommandString, ref parsedCommand);
                                if (payloadOp == PayloadOperation.RAW)
                                {
                                    outString = keepCommandString + " " + fullCommandString;
                                }
                                else if (payloadOp == PayloadOperation.SUPPRESS)
                                {
                                    outString = keepCommandString;
                                }
                                else
                                {
                                    outString = keepCommandString + " ";

                                    while (fullCommandString.Length >= 3)
                                    {
                                        try
                                        {
                                            String temp = fullCommandString.Substring(0, 2);
                                            int tempInt = (int.Parse(temp, NumberStyles.HexNumber));
                                            char tempChar = (char)tempInt;
                                            if (tempInt >= 32 && tempInt <= 126)
                                                outString += tempChar.ToString();
                                            else
                                                outString += temp + " ";
                                            fullCommandString = fullCommandString.Substring(3, fullCommandString.Length - 3);
                                        }
                                        catch
                                        {
                                            System.Diagnostics.Debug.WriteLine("char parse error");
                                        }
                                    }
                                }
                                

                                waitForParameter = false;
                                keepCommandString = "";
                            }
                        }
                        if (waitForParameter == false)
                        {                            
                            if (printOutString == true)
                                sw.WriteLine(outString);
                        }

                        printOutString = false;
                        parseAfterSuccess = false;
                        parseEverything = false;
                        ignoreToBreak = false;
                    } 
                    else if (parseEverything == true)
                    {
                        String parsedLine = line;
                      
                        parsedLine = parsedLine.Trim();
                        parsedLine = parsedLine.PadRight(16 * 3);
                        parsedLine = parsedLine.Substring(0, ((16 * 3)));
                        fullCommandString += parsedLine;                                                
                    }
                    else if (ignoreToBreak == true)
                    {
                    }
                    else if (line.StartsWith("[") == true)
                    {
                        //System.Diagnostics.Debug.WriteLine("Timestamp: " + line.ToString());
                        outString = line;
                        printOutString = false;
                        parseAfterSuccess = false;
                        parseEverything = false;
                        ignoreToBreak = false;
                        fullCommandString = "";
                    }
                    else if (line.Contains("IRP_MJ_CREATE") == true)
                    {
                        //System.Diagnostics.Debug.Write("COM port opened");
                        outString += "COM port opened";
                        printOutString = true;
                    }
                    else if (line.Contains("IRP_MJ_CLOSE") == true)
                    {
                        //System.Diagnostics.Debug.Write("COM port closed");
                        outString += "COM port closed";
                        printOutString = true;
                    }
                    else if (line.Contains("IRP_MJ_WRITE") == true)
                    {
                        //System.Diagnostics.Debug.Write("From host:");
                        //outString += "From host:";
                        lastMessageSource = COM_SOURCE.HOST;
                        parseAfterSuccess = true;
                        printOutString = true;
                    }
                    else if (line.Contains("IRP_MJ_READ") == true)
                    {
                        //System.Diagnostics.Debug.Write("From host:");
                        //outString += "From eMote:";
                        lastMessageSource = COM_SOURCE.EMOTE;
                        parseAfterSuccess = true;
                        printOutString = true;
                    }
                    else if (line.Contains("STATUS_SUCCESS") == true)
                    {
                        //System.Diagnostics.Debug.Write("found success");
                        if (parseAfterSuccess == true)
                        {
                            parseAfterSuccess = false;
                            printOutString = true;
                            parseEverything = true;
                        }
                    }
                    else if (line.Contains("IRP_MJ_DEVICE_CONTROL") == true)
                    {
                        ignoreToBreak = true;
                    }
                    else
                    {
                        System.Diagnostics.Debug.WriteLine("******** Unknown command *********" + line);
                        outString += "******** Unknown command *********";
                        printOutString = true;
                        ignoreToBreak = true;
                    }

                    line = sr.ReadLine();
                }
                sw.Flush();
                sr.Close();
                sw.Close();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("read file line: " + ex.ToString());
            }
        }
    }
}