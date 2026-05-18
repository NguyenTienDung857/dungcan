using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

namespace ReadDTCs
{
    class Program
    {
        static void Main(string[] args)
        {
            CANoe.Application app = new CANoe.Application();
            CANoe.UI gui = (CANoe.UI)app.UI;

            CANoe.Network network = (CANoe.Network)app.get_Networks("CAN");
            CANoe.Devices devices = (CANoe.Devices)network.Devices;
            CANoe.Device device = (CANoe.Device)devices["Door"];
            Diagnostic.Diagnostic diag = (Diagnostic.Diagnostic)device.Diagnostic;

            Diagnostic.DiagnosticRequest req = diag.CreateRequest("FaultMemory_ReadAllIdentified");
            if (req == null)
            {
                gui.Write.Output("Cannot create request!");
                return;
            }
            req.SetParameter("DtcStatusbyte", 0x09);
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
            int count = resp.GetComplexIterationCount("ListOfDTC");
            if (0 >= count)
            {
                gui.Write.Output("No DTC found!");
                return;
            }
            for (int i = 0; i < count; ++i)
            {
                String dtcText = (string) resp.GetComplexParameter("ListOfDTC", i, "DTC");
                String dtcStatus = (String) resp.GetComplexParameter("ListOfDTC", i, "DtcStatusbyte");
                gui.Write.Output(i.ToString() + " " + dtcText + " (" + dtcStatus + ")");
            }
        }
    }
}
