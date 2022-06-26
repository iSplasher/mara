import os
import sys
import subprocess
import itertools

def validate_pair(ob):
    try:
        if not (len(ob) == 2):
            print("Unexpected result:", ob, file=sys.stderr)
            raise ValueError
    except:
        return False
    return True

def consume(iter):
    try:
        while True: next(iter)
    except StopIteration:
        pass

def get_environment_from_batch_command(env_cmd):
    """
    Take a command (either a single command or list of arguments)
    and return the environment created after running that command.
    Note that if the command must be a batch file or .cmd file, or the
    changes to the environment will not be captured.
    """
    if not isinstance(env_cmd, (list, tuple)):
        env_cmd = [env_cmd]
    # construct the command that will alter the environment
    env_cmd = subprocess.list2cmdline(env_cmd)

    print(env_cmd)
    # create a tag so we can tell in the output when the proc is done
    tag = 'Done running command'
    # construct a cmd.exe command to do accomplish this
    cmd = 'cmd.exe /s /c "{env_cmd} && echo "{tag}" && set"'.format(**vars())
    # launch the process
    proc = os.popen(cmd)

    lines = proc.read()
    lines = lines.split(f'"{tag}"')[1]
    lines = lines.strip().splitlines()

    # define a way to handle each KEY=VALUE line
    handle_line = lambda l: l.rstrip().split('=',1)
    # parse key/values into pairs
    pairs = map(handle_line, lines)
    # make sure the pairs are valid
    valid_pairs = filter(validate_pair, pairs)
    # construct a dictionary of the pairs
    result = dict(valid_pairs)
    return result

r = os.popen(
        r'"%programfiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -latest -property installationPath'
    )


vswhere_path = r.read().strip()
if not vswhere_path:
    raise SystemExit("Could not find Visual Studio's vswhere.exe (Is Visual Studio installed?)")



vs_dev_cmd =  r'%s\Common7\Tools\VsDevCmd.bat' % vswhere_path

if len(sys.argv) > 1 and sys.argv[1] == 'x64':
   vs_dev_cmd = [vs_dev_cmd, "-arch=amd64"]

env = get_environment_from_batch_command(
       vs_dev_cmd
    )


# format for cmake intake


print(";".join(["{}|{}".format(k, v.replace(';', '\\\\\\;')) for k, v in env.items()]))