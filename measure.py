import subprocess as subprocess
import sys

if __name__ == '__main__':
    command = sys.argv[1]
    o, e = subprocess.Popen(['bash', '-c', command], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

    time_output = e.decode('utf-8')
    user_time = time_output.split('\n')[2].split('\t')[1]
    user_m = float(user_time.split('m')[0])
    user_s = float(user_time.split('m')[1][:-1].replace(',', '.'))
    total_s = user_m*60 + user_s
    print(f"Total time: {total_s}s")
