
import win32com.client
from win32api import Sleep
import time

bytestream_ReadECUidentification = [0x1A, 0x90]
bytestream_ReadDevelopmentData = [0x1A, 0x91]
bytestream_ReadSerialNumber = [0x1A, 0x92]

canoeobj = win32com.client.Dispatch("CANoe.Application");

#Please set the path where the script is located.
ScriptPath = "D:\CS0444640 - PythonDiagRaw";
configpath = ScriptPath + "\SampleConfig\SampleConfig.cfg";
if canoeobj.Configuration.Path!=ScriptPath:
 canoeobj.Open(configpath);
canoemeasurement= canoeobj.Measurement;
if canoemeasurement.Running==0:
 mCapl = canoeobj.CAPL;
 mCapl.Compile("");
 canoemeasurement.Start();
 Sleep(5000);

network = canoeobj.Networks('Comfort');
device = network.Devices('Door_Left');
diagnostic = device.Diagnostic;
print("Diagnosis: Create Request")
request = diagnostic.CreateRequestFromStream(bytestream_ReadSerialNumber);
print("Diagnosis: Send Request in Queue");
request.send(10);

while request.pending:
    print("...pending")
    time.sleep(1)

timeout = time.time()+ 15

while request.responses.Count == 0 and time.time()-timeout < 0:
    time.sleep(1)

if (request.Responses.Count != 0):
    response = request.Responses[0]
 #   lResponse = [ord(i) for i in oResponse.Stream[:]]

    lResponse = [hex(el) for el in response.Stream[:] ]

    print(lResponse)
else:
    print("no response")


canoeobj.Measurement.Stop();
#canoeobj.Quit();
#canoeobj=None;