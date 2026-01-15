import glob
import json
import os


def process_file(file_name):
    print('Parsing {}'.format(file_name))

    with open(file_name, 'r') as file:
        configuration = json.load(file)

    with open(file_name, 'w') as file:
        json.dump(configuration, file, indent=4, sort_keys=True)
        file.write('\n')


def main():
    for file_name in glob.iglob(os.path.join('..', '..', 'bin', '**', '*.json'), recursive=True):
        process_file(file_name)


if __name__ == '__main__':
    main()
