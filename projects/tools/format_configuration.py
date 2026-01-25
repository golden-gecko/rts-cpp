import glob
import json
import os
import re


def process_json_file(file_name):
    print('Parsing {}'.format(file_name))

    with open(file_name, 'r') as file:
        configuration = json.load(file)

    with open(file_name, 'w') as file:
        json.dump(configuration, file, indent=4, sort_keys=True)
        file.write('\r\n')


def format_json_configurations(path):
    print('format_json_configurations(' + path + ')')

    for file_name in glob.iglob(path, recursive=True):
        process_json_file(file_name)


def process_material_file(file_name):
    print('Parsing {}'.format(file_name))

    with open(file_name, 'r') as file:
        material = file.read()
        material = re.sub(r'specular.*', '', material)

    with open(file_name, 'w') as file:
        file.write(material)


def format_materials(path):
    print('format_materials(' + path + ')')

    for file_name in glob.iglob(path, recursive=True):
        process_material_file(file_name)


def main():
    # format_json_configurations(os.path.join('..', '..', 'bin', '**', '*.json'))
    format_materials(os.path.join('..', '..', 'bin', 'meshes', '**', '*.material'))


if __name__ == '__main__':
    main()
