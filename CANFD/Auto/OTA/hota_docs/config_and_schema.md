# Config And Schema

## Current INI state

### configuration.ini
- [OPTION]
  - `Voltage` = `12V`
  - `PortNumber` = ``
  - `PortName` = ``
  - `Stopbit` = ``
  - `Baudrate` = ``
  - `VehicleType` = `PassengerCar`
  - `Databit` = ``
  - `Parity` = ``
- [SETVALUE]
  - `DLC` = `15`
  - `externalSecurityAlgorithm` = `D:\project\CANoe\CANFD\Auto\OTA\ota\SeedKEY\HKMC_AdvancedSeedKey_Win32.dll`
  - `testpresent` = `TRUE`
  - `NM_MSG` = `FALSE`
  - `NM_TYPE` = `AliveNM`
  - `cooperate` = `FALSE`
  - `OEUK` = `NONE`
  - `NFS` = `FALSE`
  - `SourceAddress` = `ERT`
  - `GATEWAYTYPE` = `CCU1`
  - `MODELTYPE` = `GroupA(LX3,SW1,LQ2,JG1)`
### EditorConfig.ini
- [OPTION]
  - `Voltage` = `12V`
  - `VehicleType` = `PassengerCar`
  - `externalSecurityAlgorithm` = `D:\project\CANoe\CANFD\Auto\OTA\ota\SeedKEY\HKMC_AdvancedSeedKey_Win32.dll`
  - `ecucode_path` = `D:\project\CANoe\CANFD\Auto\OTA\ota\target_ecu_code_list.xml`
  - `PortNumber` = ``
  - `PortName` = ``
  - `Stopbit` = ``
  - `Baudrate` = ``
  - `Databit` = ``
  - `Parity` = ``
  - `EtherNetDeviceName` = `Realtek PCIe GbE Family Controller`
  - `EtherNetDevice` = `\Device\NPF_{7A417F5A-D677-44A0-8E16-3F3726A77494}`
- [TESTREPORT]
  - `Comment` = ``
  - `PcapSave` = `1`
  - `ReportFilePath` = `D:\project\CANoe\CANFD\Auto\OTA\LOGASC\baocao.asc`
- [ROMPACKINFO]
  - `rompack_path` = ``
  - `ecucode_path` = ``
### Testcase2.ini
- [Rollback]
  - `TC_TAR_01041` = ``
  - `TC_TAR_01040` = `no`
  - `TC_TAR_01045` = `no`
  - `TC_TAR_01050` = `no`
  - `TC_TAR_01053` = `no`
  - `TC_TAR_01055` = `no`
  - `R-ROM Path` = ``
- [Wired]
  - `TC_TAR_03010` = `no`
  - `TC_TAR_03020` = `no`
  - `TC_TAR_03030` = `no`
  - `TransferData_Invert_area` = `TransferData Area:1`
- [softwareInformation]
  - `TC_TAR_02010` = `no`
- [Background-Transfer]
  - `TC_TAR_01010` = `no`
  - `TC_TAR_01011` = `no`
  - `TransferData_Invert_area` = `TransferData Area:1`
- [OTAReady]
  - `TC_TAR_02020-02050` = `no`
  - `TC_TAR_02020-02051` = `no`
  - `VehicleType` = `PassengerCar`
  - `NMMessage_Type` = `AliveNM`
  - `NMMessage_Time` = `30`
  - `TestECU` = `ALL(11bit)`
- [UpdateAndFinish]
  - `TC_TAR_01020` = `no`
  - `TC_TAR_01021` = `no`
  - `TC_TAR_01022` = `no`
  - `TC_TAR_01023` = `no`
  - `TransferData_Invert_area` = `TransferData Area:1`
- [Reset]
  - `TC_TAR_01030` = `no`
  - `TC_TAR_01035` = `no`
  - `TransferData_Invert_area` = `TransferData Area:1`
- [General]
  - `TC_TAR_01060` = `no`
  - `TC_TAR_01070` = `no`
  - `R-ROM Path` = ``
  - `TC_TAR_02060` = `no`
  - `TC_TAR_02070` = `no`
- [OTAFunctionSafety]
  - `TC_TAR_01080` = `no`
  - `TC_TAR_01081` = `no`
  - `TC_TAR_01082_01084` = `no`
  - `TC_TAR_01083_01084` = `no`
  - `TC_TAR_01082_01085` = `no`
  - `TC_TAR_01083_01085` = `no`
### version.ini
- [VERSION]
  - `STUDIO` = `0.1.1.116`
  - `UPDATER` = `0.1.1.088`
  - `RESOURCE` = `0.1.1.119`
  - `UPDATEMANAGER` = `0.1.1.013`
  - `MANUAL` = `0.1.1.004`

## Property XML summary

### root/service/commonService
- `serviceId` type=`input` default=`` values=``
- `confirmPositiveResponse` type=`combo` default=`yes` values=`yes|no`
- `serviceId` type=`input` default=`` values=``
- `confirmPositiveResponse` type=`combo` default=`yes` values=`yes|no`
### root/service/configuration
- `protocol` type=`combo` default=`ES95480-00(CAN)` values=`ES95480-00(CAN)|ES95480-02(CAN-FD)|ES96595-05(Ethernet)|ES95400-30(LS-CAN)`
- `application` type=`input` default=`ES98765-01(OTA)` values=``
- `moduleId` type=`input` default=`` values=``
- `testerId` type=`input` default=`` values=``
- `requestType` type=`combo` default=`physical` values=`physical`
- `stminTx` type=`combo` default=`Auto(0xFFFF)` values=`Auto(0xFFFF)|100us(0xF1)|200us(0xF2)|300us(0xF3)|400us(0xF4)|500us(0xF5)|600us(0xF6)|700us(0xF7)|800us(0xF8)|900us(0xF9)|0ms(0x00)|1ms(0x01)|2ms(0x02)|3ms(0x03)|4ms(0x04)|5ms(0x05)|6ms(0x06)|7ms(0x07)|8ms(0x08)|9ms(0x09)|10ms(0x0A)`
- `p2CanServerMax` type=`input` default=`50` values=``
- `p2StarCanServerMax` type=`input` default=`5000` values=``
- `DLC` type=`combo` default=`8` values=`8(8)|9(12)|10(16)|11(20)|12(24)|13(32)|14(48)|15(64)`
- `sourceIP` type=`input` default=`10.0.128.0` values=``
- `sourcePort` type=`input` default=`` values=``
- `destinationIP` type=`input` default=`` values=``
- `destinationPort` type=`input` default=`13404` values=``
- `gatewayType` type=`combo` default=`` values=`CCU|ICU`
- `dataBitRate` type=`combo` default=`2` values=`1|2`
- `NRC78Repetitiontimeout` type=`input` default=`300` values=``
- `stminTx` type=`combo` default=`Auto(0xFFFF)` values=`Auto(0xFFFF)|100us(0xF1)|200us(0xF2)|300us(0xF3)|400us(0xF4)|500us(0xF5)|600us(0xF6)|700us(0xF7)|800us(0xF8)|900us(0xF9)|0ms(0x00)|1ms(0x01)|2ms(0x02)|3ms(0x03)|4ms(0x04)|5ms(0x05)|6ms(0x06)|7ms(0x07)|8ms(0x08)|9ms(0x09)|10ms(0x0A)`
- `background_stminTx` type=`combo` default=`Auto(0xFFFF)` values=`1ms(0x01)|2ms(0x02)|3ms(0x03)|4ms(0x04)|5ms(0x05)|6ms(0x06)|7ms(0x07)|8ms(0x08)|9ms(0x09)|10ms(0x0A)`
- `p2CanServerMax` type=`input` default=`50` values=``
- `p2StarCanServerMax` type=`input` default=`5000` values=``
- `NRC78Repetitiontimeout` type=`input` default=`300` values=``
### root/service/diagnosticSessionControl
- `diagnosticSessionType` type=`input` default=`` values=``
- `aliveCheckMessageInterval` type=`combo` default=`60` values=`1|2|60`
- `diagnosticSessionType` type=`input` default=`` values=``
- `aliveCheckMessageInterval` type=`combo` default=`60` values=`1|2|60`
- `skipTask` type=`combo` default=`` values=``
### root/service/controlDTCSetting
- `dtcSettingType` type=`input` default=`` values=``
- `dtcSettingType` type=`input` default=`` values=``
### root/service/communicationControl
- `controlType` type=`input` default=`` values=``
- `communicationType` type=`input` default=`` values=``
- `controlType` type=`input` default=`` values=``
- `communicationType` type=`input` default=`` values=``
### root/service/securityAccess
- `algorithm` type=`combo` default=`ASK(AUTOEVER)` values=`ASK(AUTOEVER)|SEED_KEY(INCA)|SEED_KEY(현대AUTOSAR)`
- `vehicle` type=`input` default=`` values=``
- `number` type=`input` default=`` values=``
- `algorithm` type=`combo` default=`ASK(AUTOEVER)` values=`ASK(AUTOEVER)`
- `vehicle` type=`input` default=`` values=``
- `number` type=`input` default=`` values=``
- `skipTask` type=`combo` default=`` values=``
### root/service/requestSeed
- `accessMode` type=`input` default=`11` values=``
- `accessMode` type=`input` default=`11` values=``
### root/service/sendKey
- `accessMode` type=`input` default=`12` values=``
- `accessMode` type=`input` default=`12` values=``
### root/service/routineControl
- `routineControlType` type=`input` default=`` values=``
- `routineIdentifier` type=`input` default=`` values=``
- `routineControlOptionRecord` type=`input` default=`` values=``
- `routineControlType` type=`input` default=`` values=``
- `routineIdentifier` type=`input` default=`` values=``
- `routineControlOptionRecord` type=`input` default=`` values=``
### root/service/requestDownload
- `dataFormatIdentifier` type=`input` default=`` values=``
- `addressAndLengthFormatIdentifier` type=`input` default=`` values=``
- `memorySize` type=`input` default=`` values=``
- `memoryAddress` type=`input` default=`` values=``
- `dataFormatIdentifier` type=`input` default=`` values=``
- `addressAndLengthFormatIdentifier` type=`input` default=`` values=``
- `memorySize` type=`input` default=`` values=``
- `memoryAddress` type=`input` default=`` values=``
### root/service/transferData
- `maxNumberOfBlockLength` type=`input` default=`` values=``
- `seekAddress` type=`input` default=`` values=``
- `writeSize` type=`input` default=`` values=``
- `maxNumberOfBlockLength` type=`input` default=`` values=``
- `seekAddress` type=`input` default=`` values=``
- `writeSize` type=`input` default=`` values=``
### root/service/requestVehicleInformation
- `infoType` type=`input` default=`` values=``
- `infoType` type=`input` default=`` values=``
### root/service/ecuReset
- `resetMode` type=`input` default=`0x01` values=``
- `resetMode` type=`input` default=`0x01` values=``
### root/service/frame
- `transmitdata` type=`input` default=`0x00` values=``
- `transmitdata` type=`input` default=`0x00` values=``
### root/service/readDataByIdentifier
- `dataIdentifier` type=`input` default=`0xF1B1` values=``
- `startPos` type=`input` default=`` values=``
- `display` type=`combo` default=`` values=`ascii`
- `format` type=`input` default=`###` values=``
- `dataIdentifier` type=`input` default=`0xF1B1` values=``
- `startPos` type=`input` default=`` values=``
- `endPos` type=`input` default=`` values=``
- `display` type=`combo` default=`` values=`ascii`
### root/service/delay
- `time` type=`input` default=`` values=``
- `time` type=`input` default=`` values=``
### root/service/selectByArea-rule
- `areaFormatIdentifier` type=`combo` default=`0x0A` values=`0x0A|0x0B`
- `areaFormatIdentifier` type=`combo` default=`0x0A` values=`0x0A|0x0B`
### root/service/clearDiagnosticInformation
- `groupOfDTC` type=`input` default=`0xFFFFFF` values=``
- `groupOfDTC` type=`input` default=`0xFFFFFF` values=``
### root/service/readDTCInformation
- `subFunction` type=`input` default=`` values=``
- `extendedInfo` type=`input` default=`` values=``
- `subFunction` type=`input` default=`` values=``
- `extendedInfo` type=`input` default=`` values=``
### root/service-ES95485/commonService
- `serviceId` type=`input` default=`` values=``
- `confirmPositiveResponse` type=`combo` default=`yes` values=`yes|no`
### root/service-ES95485/configuration
- `protocol` type=`combo` default=`ES95480-00(CAN)` values=`ES95480-00(CAN)|ES95480-02(CAN-FD)|ES96595-05(Ethernet)`
- `application` type=`input` default=`ES95485-00` values=``
- `moduleId` type=`input` default=`` values=``
- `testerId` type=`input` default=`` values=``
- `requestType` type=`combo` default=`physical` values=`physical`
- `stminTx` type=`combo` default=`Auto(0xFFFF)` values=`Auto(0xFFFF)|100us(0xF1)|200us(0xF2)|300us(0xF3)|400us(0xF4)|500us(0xF5)|600us(0xF6)|700us(0xF7)|800us(0xF8)|900us(0xF9)|0ms(0x00)|1ms(0x01)|2ms(0x02)|3ms(0x03)|4ms(0x04)|5ms(0x05)|6ms(0x06)|7ms(0x07)|8ms(0x08)|9ms(0x09)|10ms(0x0A)`
- `p2CanServerMax` type=`input` default=`50` values=``
- `p2StarCanServerMax` type=`input` default=`5000` values=``
- `DLC` type=`combo` default=`8` values=`8(8)|9(12)|10(16)|11(20)|12(24)|13(32)|14(48)|15(64)`
- `sourceIP` type=`input` default=`10.0.128.0` values=``
- `sourcePort` type=`input` default=`` values=``
- `destinationIP` type=`input` default=`` values=``
- `destinationPort` type=`input` default=`13404` values=``
- `gatewayType` type=`combo` default=`CCU` values=`CCU|ICU`
### root/service-ES95485/diagnosticSessionControl
- `diagnosticSessionType` type=`input` default=`` values=``
### root/service-ES95485/controlDTCSetting
- `dtcSettingType` type=`input` default=`` values=``
### root/service-ES95485/communicationControl
- `controlType` type=`input` default=`` values=``
- `communicationType` type=`input` default=`` values=``
### root/service-ES95485/securityAccess
- `algorithm` type=`combo` default=`ASK(AUTOEVER)` values=`ASK(AUTOEVER)|SEED_KEY(INCA)|SEED_KEY(현대AUTOSAR)`
- `vehicle` type=`input` default=`` values=``
- `number` type=`input` default=`` values=``
### root/service-ES95485/requestSeed
- `accessMode` type=`input` default=`11` values=``
### root/service-ES95485/sendKey
- `accessMode` type=`input` default=`12` values=``
### root/service-ES95485/routineControl
- `routineIdentifier` type=`input` default=`` values=``
- `routineControlOptionRecord` type=`input` default=`` values=``
### root/service-ES95485/requestDownload
- `memoryAddress` type=`input` default=`` values=``
- `dataFormatIdentifier` type=`input` default=`` values=``
- `memorySize` type=`input` default=`` values=``
### root/service-ES95485/transferData
- `maxNumberOfBlockLength` type=`input` default=`` values=``
- `seekAddress` type=`input` default=`` values=``
- `writeSize` type=`input` default=`` values=``
### root/service-ES95485/ecuReset
- `resetMode` type=`input` default=`0x01` values=``
### root/service-ES95485/frame
- `transmitdata` type=`input` default=`0x00` values=``
### root/service-ES95485/readDataByIdentifier
- `dataIdentifier` type=`input` default=`` values=``
- `startPos` type=`input` default=`` values=``
- `display` type=`combo` default=`` values=`ascii`
- `format` type=`input` default=`###` values=``
### root/service-ES95485/delay
- `time` type=`input` default=`` values=``
### root/service-ES95485/clearDiagnosticInformation
- `groupOfDTC` type=`input` default=`0xFFFFFF` values=``
### root/service-ES95485/readDTCInformation
- `subFunction` type=`input` default=`` values=``
- `extendedInfo` type=`input` default=`` values=``
### root/common/instance
- `vehicleName` type=`input` default=`` values=``
- `ecuName` type=`input` default=`` values=``
- `hwPartNum` type=`input` default=`` values=``
- `swUnitID` type=`combo` default=`01` values=`01|02|03|04|05|06|07|08|09|0A|0B|0C|0E|0D|0F`
- `sourceVersion` type=`input` default=`` values=``
- `targetVersion` type=`input` default=`` values=``
- `dbVersion` type=`input` default=`` values=``
- `rom` type=`file` default=`` values=``
- `comment` type=`input` default=`` values=``
- `creationDate` type=`input` default=`` values=``
- `askHashValue` type=`input` default=`` values=``
- `ota2compatibility` type=`input` default=`false` values=``
- `updateType` type=`input` default=`` values=``
- `redundancy` type=`input` default=`` values=``
### root/service/whatsNew
- `comment` type=`input` default=`` values=``
### root/common/instance/docInfo
- `documentVersion` type=`input` default=`` values=``
### root/common/instance/vehicleInfo
- `vehicleName` type=`input` default=`` values=``
- `ecuName` type=`input` default=`` values=``
### root/common/instance/specInfo
- `updateStandard` type=`combo` default=`` values=`TRUE|FALSE`
### root/common/instance/swTypeInfo
- `ucm` type=`combo` default=`` values=`TRUE|FALSE`
- `bootloader` type=`combo` default=`` values=`TRUE|FALSE`
- `swType` type=`combo` default=`` values=`Firmware|Application`
- `executionType` type=`combo` default=`` values=`Install|Update|Remove`
### root/common/instance/packageInfo
- `gate` type=`combo` default=`` values=`Dev|Sop`
- `isEnableOTA` type=`combo` default=`` values=`TRUE|FALSE`
- `updateType` type=`combo` default=`` values=`MemorySwap|DeltaMemorySwap`
- `redundancy` type=`combo` default=`N/A` values=`N/A|Background|Normal`
### root/common/instance/commInfo
- `networkType` type=`combo` default=`` values=`CAN|CANFD|Ethernet`
- `ethProtocol` type=`combo` default=`` values=`DoIP|Ethdiag`
- `requestId` type=`input` default=`` values=``
- `responseId` type=`input` default=`` values=``
- `sourceIP` type=`input` default=`` values=``
- `sourcePort` type=`input` default=`` values=``
- `destinationIP` type=`input` default=`` values=``
- `destinationPort` type=`input` default=`` values=``
- `p6Time` type=`input` default=`50` values=``
- `dataBitRate` type=`combo` default=`2` values=`1|2`
### root/common/instance/romInfo
- `rom` type=`file` default=`` values=``
### root/common/instance/versionInfo
- `partNumber` type=`input` default=`` values=``
- `unit` type=`combo` default=`01` values=`01|02|03|04|05|06|07|08|09|0A|0B|0C|0D|0E|0F`
- `sourceVersion` type=`input` default=`` values=``
- `targetVersion` type=`input` default=`` values=``
- `dbVersion` type=`input` default=`` values=``
### root/common/instance/creationDate
- `creationDate` type=`input` default=`` values=``
### root/common/instance/askHashValue
- `askHashValue` type=`input` default=`` values=``

## Schema families

- `Schema\Config\build.xsd` elements=0 complexTypes=0
- `Schema\Config\CCU1\build.xsd` elements=0 complexTypes=0
- `Schema\Config\CCU1\errorRule.xsd` elements=8 complexTypes=0
- `Schema\Config\CCU1\information.xsd` elements=7 complexTypes=0
- `Schema\Config\CCU1\NonOTACANFDSchema.xsd` elements=43 complexTypes=0
- `Schema\Config\CCU1\NonOTASchema.xsd` elements=43 complexTypes=0
- `Schema\Config\CCU1\OTASchema.xsd` elements=46 complexTypes=0
- `Schema\Config\CCU1\software.xsd` elements=0 complexTypes=0
- `Schema\Config\CCU2\build.xsd` elements=0 complexTypes=0
- `Schema\Config\CCU2\information.xsd` elements=13 complexTypes=0
- `Schema\Config\CCU2\OTA_Background_APP_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Background_APP_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Background_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Background_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Background_UCM_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Background_UCM_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Basic_APP_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Basic_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Basic_UCM_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Normal_APP_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Normal_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\CCU2\OTA_Normal_UCM_Schema.xsd` elements=34 complexTypes=5
- `Schema\Config\errorRule.xsd` elements=8 complexTypes=0
- `Schema\Config\information.xsd` elements=7 complexTypes=0
- `Schema\Config\NonOTACANFDSchema.xsd` elements=43 complexTypes=0
- `Schema\Config\NonOTASchema.xsd` elements=43 complexTypes=0
- `Schema\Config\OTASchema.xsd` elements=46 complexTypes=0
- `Schema\Config\software.xsd` elements=0 complexTypes=0
- `Schema\Rompack\CCU1\NonOTARompackSchema.xsd` elements=43 complexTypes=0
- `Schema\Rompack\CCU1\RompackBuild.xsd` elements=0 complexTypes=0
- `Schema\Rompack\CCU1\RompackError.xsd` elements=8 complexTypes=0
- `Schema\Rompack\CCU1\RompackInformation.xsd` elements=8 complexTypes=0
- `Schema\Rompack\CCU1\RompackSchema.xsd` elements=46 complexTypes=0
- `Schema\Rompack\CCU1\RompackSoftware.xsd` elements=0 complexTypes=0
- `Schema\Rompack\CCU2\build.xsd` elements=0 complexTypes=0
- `Schema\Rompack\CCU2\information.xsd` elements=14 complexTypes=0
- `Schema\Rompack\CCU2\OTA_Background_APP_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Background_APP_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Background_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Background_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Background_UCM_Rev_Schema.xsd` elements=34 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Background_UCM_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Basic_APP_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Basic_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Basic_UCM_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Normal_APP_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Normal_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\CCU2\OTA_Normal_UCM_Schema.xsd` elements=33 complexTypes=5
- `Schema\Rompack\NonOTARompackSchema.xsd` elements=43 complexTypes=0
- `Schema\Rompack\RompackBuild.xsd` elements=0 complexTypes=0
- `Schema\Rompack\RompackError.xsd` elements=8 complexTypes=0
- `Schema\Rompack\RompackInformation.xsd` elements=8 complexTypes=0
- `Schema\Rompack\RompackSchema.xsd` elements=46 complexTypes=0
- `Schema\Rompack\RompackSoftware.xsd` elements=0 complexTypes=0

## Vehicle/controller info

- status_code: `200`
- car_count: `41`
- car `AI4` id=`278`
- car `BC4` id=`272`
- car `BJ1` id=`277`
- car `BO4` id=`450`
- car `CJ1` id=`274`
- car `CL4` id=`144`
- car `CN8` id=`276`
- car `CT1` id=`146`
- car `EA1` id=`408`
- car `EC1` id=`454`
- car `GT1` id=`286`
- car `HE1` id=`307`
- car `JG` id=`186`
- car `JW2` id=`443`
- car `JX2` id=`287`
- car `KA5` id=`442`
- car `LQ2` id=`165`
- car `LT2` id=`402`
- car `LW1` id=`378`
- car `LX3` id=`243`
- car `ME1` id=`105`
- car `NE2` id=`283`
- car `NH2` id=`145`
- car `NI1` id=`422`
- car `NQ6` id=`279`
- car `NX5` id=`271`
- car `OV1k` id=`269`
- car `QU2` id=`367`
- car `QV1` id=`285`
- car `QY2` id=`280`
- car `RG4` id=`282`
- car `SP3` id=`254`
- car `SV1` id=`182`
- car `SW1` id=`227`
- car `SX3` id=`351`
- car `TE1` id=`281`
- car `TK1` id=`247`
- car `TV1` id=`444`
- car `UZ1` id=`284`
- car `XP2` id=`413`

Raw data: `config_and_schema.json`.
