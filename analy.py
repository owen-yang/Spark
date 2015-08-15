import csv
with open('data.tsv','rb') as tsvin, open('new.csv', 'wb') as csvout:
    tsvin = csv.reader(tsvin, delimiter='\t')
    csvout = csv.writer(csvout)

    rows = list()
    for line in tsvin:
        arr = line[0].split(' ')[0:3]
        print arr[0]+',',arr[1]+',',arr[2]+','

        # count = int(row[4])
        # if count > 0:
        #     csvout.writerows([row[2:4] for _ in xrange(count)])

