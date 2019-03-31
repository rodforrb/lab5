# return priority of task so they can be compared
def cmpTask(task):
    return int(task.split(', ')[1])

tasks = []
with open('schedule.txt') as fileIn:
    lines = fileIn.readlines()
    for line in lines:
        tasks.append(line)

with open('schedule2.txt', 'w') as fileOut:
    for t in sorted(tasks, key=cmpTask):
        fileOut.write(t)

