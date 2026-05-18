using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

namespace ReqResp
{
    class Program
    {
        static void Main(string[] args)
        {
            Byte[] rawRequest=new Byte[2] {0x10, 0x03}; // raw bytes for Start Extended Session
            String s;

            CANoe.Application app = new CANoe.Application();
            CANoe.UI gui = (CANoe.UI)app.UI;

            CANoe.Network network = (CANoe.Network)app.get_Networks("DIAGCAN"); // Network name
            CANoe.Devices devices = (CANoe.Devices)network.Devices;
            CANoe.Device device = (CANoe.Device)devices["Door"];    // CDD (ECU) Qualifier
            Diagnostic.Diagnostic diag = (Diagnostic.Diagnostic)device.Diagnostic;

            // ---------------------------------------------------------------------------------
            // Send Request using qualifier from CDD
            //
            //            Diagnostic.DiagnosticRequest req = diag.CreateRequest("DefaultSession_Start");
            //
            // or
            // 
            // as Raw Request using raw bytes:
            //
            Diagnostic.DiagnosticRequest req = diag.CreateRequestFromStream(rawRequest);
            if (req == null)
            {
                gui.Write.Output("Cannot create request!");
                return;
            }
            req.Send();
            while( req.Pending)
            {
                System.Threading.Thread.Sleep( 10);
            }
            Diagnostic.DiagnosticResponse resp = req.Responses(1);
            if (resp == null)
            {
                gui.Write.Output("No response received!");
                return;
            }
            else
                gui.Write.Output("Response received.");

            // ---------------------------------------------------------------------------------
            // Send 2nd Request using qualifier from CDD
            // ---------------------------------------------------------------------------------

            Diagnostic.DiagnosticRequest req2 = diag.CreateRequest("SerialNumber_Read");
            if (req2 == null)
            {
                gui.Write.Output("Cannot create request!");
                return;
            }
            req2.Send();
            while (req2.Pending)
            {
                System.Threading.Thread.Sleep(10);
            }
            Diagnostic.DiagnosticResponse resp2 = req2.Responses(1);
            if (resp == null)
            {
                gui.Write.Output("No response received!");
                return;
            }
            else
            {
                s = resp2.GetParameter("SerialNumber");
                gui.Write.Output("Response received: SerialNumber is"+s);
            }            
        }
    }
}
