from subprocess import Popen, PIPE
import os

path = "Salesman01.exe"
real_path = os.path.realpath(path)

points = [50, 50, 100, 50, 100, 100, 50, 100]
nPoints = 13

# commands = ["--inputPoints", str(len(points)/2)]
commands = ["--points", str(nPoints)]
# commands = []
sales_call = Popen([real_path, *commands], stdout=PIPE, stdin = PIPE)

# for p in points:
#     sales_call.stdin.write(p.to_bytes((p.bit_length() + 7)//8, 'big'))
#     sales_call.stdin.write(b"\n")
#
# sales_call.stdin.flush()


output = [out.strip() for out in sales_call.stdout.readlines()]
print(output)



indexes = [int(out) for out in output[:nPoints]]
points = [int(out) for out in output[nPoints:nPoints+2*nPoints]]
# print(type(result[0].strip()))

print("indizes: ", indexes)
print("points: ", points)
print("minimum distance: ", float(output[-3]))
print("time for best, time total: ", int(output[-2]), int(output[-1]))