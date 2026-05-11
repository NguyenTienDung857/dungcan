"""
Convert 2.5 generation CAPL test files to 2.0 generation format.
Transformation rules:
  1. Encoding: 65001 -> 1252
  2. Message declarations: CAN FD names -> CAN names
  3. Variable references: _HU_BLTN_CAM_02_200ms -> _HU_BLTN_CAM_PE_00_200ms
  4. CaptureGraphics: 2-param with snprintf -> 1-param with ./Husign.jpg
  5. Report filename: 2.5 generation -> 2.0 generation
  6. OWD_032 special: local message HU_BLTN_CAM_01_00ms -> HU_BLTN_CAM_E_01
"""

import re
import os

base_dir = r"d:\project\CAN\[CANoe] CAN\Auto\OWD"
files_to_convert = [17, 18, 19, 20, 26, 27, 28, 29, 30, 31, 32]

def convert_file(num):
    src = os.path.join(base_dir, f"OWD_{num:03d} - 2.5 generation.can")
    dst = os.path.join(base_dir, f"OWD_{num:03d} - 2.0 generation.can")

    with open(src, 'r', encoding='utf-8') as f:
        content = f.read()

    # 1. Encoding declaration
    content = content.replace('/*@!Encoding:65001*/', '/*@!Encoding:1252*/')

    # 2. Comment: CAN FD -> CAN
    content = content.replace('// Message declarations for CAN FD', '// Message declarations for CAN')

    # 3. Global message declarations
    content = content.replace(
        'message HU_BLTN_CAM_02_200ms _HU_BLTN_CAM_02_200ms;',
        'message HU_BLTN_CAM_PE_00_200ms _HU_BLTN_CAM_PE_00_200ms;'
    )
    content = content.replace(
        'message HU_BLTN_CAM_01_00ms _HU_BLTN_CAM_01_00ms;',
        'message HU_BLTN_CAM_E_00 _HU_BLTN_CAM_E_00;\n  message HU_BLTN_CAM_E_01 _HU_BLTN_CAM_E_01;'
    )
    content = content.replace(
        'message HU_CLOCK_01_1000ms _HU_CLOCK_01_1000ms;',
        'message HU_CLOCK_PE_02 _HU_CLOCK_PE_02;'
    )

    # 4. OWD_032 special: local message declarations in Send_TML_Rec_Req functions
    # "message HU_BLTN_CAM_01_00ms _HU_BLTN_CAM_E_01;" -> "message HU_BLTN_CAM_E_01 _HU_BLTN_CAM_E_01;"
    if num == 32:
        content = content.replace(
            'message HU_BLTN_CAM_01_00ms _HU_BLTN_CAM_E_01;',
            'message HU_BLTN_CAM_E_01 _HU_BLTN_CAM_E_01;'
        )
        # Also fix the comment referencing HU_BLTN_CAM_01_00ms in function descriptions
        content = content.replace(
            'via HU_BLTN_CAM_01_00ms',
            'via HU_BLTN_CAM_E_01'
        )

    # 5. Variable references in code (Enter/Exit BLTN_CAM functions)
    content = content.replace('_HU_BLTN_CAM_02_200ms.', '_HU_BLTN_CAM_PE_00_200ms.')
    content = content.replace('output(_HU_BLTN_CAM_02_200ms)', 'output(_HU_BLTN_CAM_PE_00_200ms)')

    # 6. CaptureGraphics function definition replacement
    # Pattern A: Standard with snprintf (most files)
    capture_func_pattern_a = re.compile(
        r'void CaptureGraphics\(char signalList\[\], char filename\[\]\)\s*\{\s*'
        r'char filepath\[512\];\s*'
        r'snprintf\(filepath, elcount\(filepath\), "D:\\\\project\\\\CAN FD\\\\.*?\\\\%s", filename\);\s*'
        r'write\(">>> Capturing screenshot: %s <<<", filepath\);\s*'
        r'testWaitForTimeout\(3000\);\s*'
        r'testReportAddWindowCapture\("([^"]+)", signalList, "Screenshot", filepath\);\s*'
        r'write\(">>> Screenshot captured <<<"\);\s*'
        r'\}',
        re.DOTALL
    )

    def replace_capture_func_a(match):
        window_name = match.group(1)
        return (
            'void CaptureGraphics(char signalList[])\n'
            '{\n'
            '  write(">>> Capturing screenshot <<<");\n'
            '  testWaitForTimeout(3000);\n'
            f'  testReportAddWindowCapture("{window_name}", signalList, "Screenshot", "./Husign.jpg");\n'
            '  write(">>> Screenshot captured <<<");\n'
            '}'
        )

    content, count_a = capture_func_pattern_a.subn(replace_capture_func_a, content)

    # Pattern B: OWD_017 simple pattern (no snprintf)
    if count_a == 0:
        capture_func_pattern_b = re.compile(
            r'void CaptureGraphics\(char signalList\[\], char filename\[\]\)\s*\{\s*'
            r'testWaitForTimeout\(3000\);\s*'
            r'testReportAddWindowCapture\("([^"]+)", signalList, "Screenshot", filename\);\s*'
            r'write\(">>> Screenshot captured: %s <<<", filename\);\s*'
            r'\}',
            re.DOTALL
        )

        def replace_capture_func_b(match):
            window_name = match.group(1)
            return (
                'void CaptureGraphics(char signalList[])\n'
                '{\n'
                '  testWaitForTimeout(3000);\n'
                f'  testReportAddWindowCapture("{window_name}", signalList, "Screenshot", "./Husign.jpg");\n'
                '  write(">>> Screenshot captured <<<");\n'
                '}'
            )

        content, count_b = capture_func_pattern_b.subn(replace_capture_func_b, content)
        if count_b == 0:
            print(f"  WARNING: CaptureGraphics function not matched for OWD_{num:03d}")

    # 7. CaptureGraphics call sites: remove the second (filename) parameter
    # CaptureGraphics("signals", "filename.jpg") -> CaptureGraphics("signals")
    content = re.sub(
        r'CaptureGraphics\(("[^"]*"),\s*"[^"]*"\)',
        r'CaptureGraphics(\1)',
        content
    )

    # 8. Report filename: 2.5 generation -> 2.0 generation
    content = content.replace('2.5 generation_report', '2.0 generation_report')

    # Write output file in cp1252 encoding
    with open(dst, 'w', encoding='cp1252') as f:
        f.write(content)

    print(f"Created: OWD_{num:03d} - 2.0 generation.can")
    return dst


# Run conversion
print("=" * 60)
print("Converting 2.5 generation files to 2.0 generation")
print("=" * 60)

created_files = []
for num in files_to_convert:
    try:
        dst = convert_file(num)
        created_files.append(dst)
    except Exception as e:
        print(f"ERROR converting OWD_{num:03d}: {e}")

print()
print(f"Successfully created {len(created_files)} files.")
print("=" * 60)
