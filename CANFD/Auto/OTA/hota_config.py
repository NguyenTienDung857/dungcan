import os


def find_hota_config_file(script_dir, config_folder):
    """Return the only .hcfg-r file inside the requested H-OTA config folder."""
    config_dir = os.path.join(script_dir, config_folder)
    if not os.path.isdir(config_dir):
        raise FileNotFoundError(f"H-OTA config folder not found: {config_dir}")

    config_files = sorted(
        os.path.join(config_dir, file_name)
        for file_name in os.listdir(config_dir)
        if file_name.lower().endswith(".hcfg-r")
        and os.path.isfile(os.path.join(config_dir, file_name))
    )

    if not config_files:
        raise FileNotFoundError(f"No .hcfg-r file found in H-OTA config folder: {config_dir}")

    if len(config_files) > 1:
        file_names = ", ".join(os.path.basename(path) for path in config_files)
        raise RuntimeError(
            f"Multiple .hcfg-r files found in H-OTA config folder {config_dir}: {file_names}"
        )

    print(f"Using H-OTA config file: {config_files[0]}")
    return config_files[0]
