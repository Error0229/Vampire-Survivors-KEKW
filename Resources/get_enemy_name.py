import os

# get all file in certin directory
files = os.listdir('enemy/')

names = ['']
for filename in files:
    if (filename.endswith('.bmp')):
        name = filename.split('_')[0]
        if(name != names[-1]):
            names.append(name)

names.pop(0)
for id, name in enumerate(names):
    #print(f'{id}, {name}')
    #print(f'{id}')
    print(f'{name}')
