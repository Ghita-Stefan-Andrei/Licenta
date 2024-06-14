import argparse

def createCLArguments():
    """
    Create command-line arguments for specifying input source and file path.

    Returns:
    argparse.Namespace: An object containing the parsed command-line arguments.
    """
    parser = argparse.ArgumentParser(description = "Select in serial setting are read from file or inserted manually")
    parser.add_argument("--source", choices = ['keyboard', 'file'], required = True, help = "Specify the input source: 'keyboard' for input from keyboard, 'file' for input from a file.")
    parser.add_argument("--filePath", type = str)
    parser.add_argument("--outputType", choices = ['csv', 'cmd'], required = True, type = str)
    return parser.parse_args()