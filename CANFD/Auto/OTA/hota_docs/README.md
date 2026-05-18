# Tai lieu H-OTA Studio

Generated: 2026-05-18T11:27:37

## Muc tieu

Bo tai lieu nay luu cau truc H-OTA Studio tren may nay de AI agent sau nay co the code automation bang selector thay vi toa do.

Note: screenshot PNG va log tam cua cac lan scan da duoc xoa de giu bo nay thanh tai lieu text/JSON de AI doc truc tiep.

## Ket qua chinh

- Shortcut: `D:\project\CANoe\CANFD\Auto\OTA\H-OTA Studio.lnk`
- Shortcut target: `C:\Windows\Installer\{08463601-4A22-4255-956E-19A04EC5DA92}\_2BD3CFB986DC9672563360.exe`
- Installed folder: `C:\Program Files (x86)\GIT\H-OTA Studio GEN2`
- H-OTA runtime windows found: 1
- Output windows found: 1
- Existing OTA Python runner files parsed: 33
- Property definitions parsed: 179
- XSD schema files summarized: 54

## Tai lieu con

- `runtime_ui_scan.md`: cua so/runtime controls doc duoc tu Win32/UIA.
- `automation_selectors.md`: selector va flow Python dang dung trong repo.
- `installed_structure.md`: executable, doc, schema, template, procedure, testcase resource.
- `config_and_schema.md`: ini, property XML, XSD, vehicle/controller metadata.
- `hota_full_ui_dump.txt`: full plain-text dump similar to the older `ui_dump.txt`, including layout, selectors, combo options, click probes, exhaustive click report, and runner selectors.
- `hota_exhaustive_click_dump.txt`: controlled click/toggle report for the disconnected H-OTA session, including skipped and disabled controls.
- `limitations.md`: phan nao da test/click, phan nao bi chan boi quyen Windows.

## Nguyen tac automation

- Uu tien UIA/Win32 selector: title, auto_id, control_type, child window id, class name.
- Khong dung toa do neu control co selector on dinh.
- Nut `Start` la thao tac chay update that, chi invoke trong runner co CANoe/AppState va Output polling.
- Neu H-OTA chay elevated, Python cung phai chay Administrator va nen dung 32-bit Python cho app 32-bit.
