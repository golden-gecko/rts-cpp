import glob
import json
import sys


def main():
    for file_name in glob.iglob('bin/**/*.json', recursive=True):
        process_file(file_name)


def process_file(file_name):
    print('Parsing {}'.format(file_name))

    with open(file_name, 'r') as file:
        try:
            configuration = json.load(file)
        except Exception as e:
            print('    {}'.format(e))
            sys.exit(-1)
        else:
            with open(file_name, 'w') as file:
                try:
                    json.dump(configuration, file, indent=4, sort_keys=True)
                except Exception as e:
                    print('    {}'.format(e))
                    sys.exit(-1)


if __name__ == '__main__':
    main()
