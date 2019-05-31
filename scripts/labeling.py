import os, sys

path = "final_senses/"

files = os.listdir(path)

records = {}

for file in files:
    with open(os.path.join(path, file), 'r', encoding="utf-8", errors="ignore") as fp:
        entries = fp.readlines()

    clusters = {}
    for entry in entries:
        entry = entry.strip()
        instance = entry.split("\t")[0]
        cluster = entry.split("\t")[2]

        
        if cluster not in clusters.keys():
            clusters[cluster] = []

        if instance not in clusters[cluster]:
            clusters[cluster].append(instance)
    
    senses = {}
    record = {}
    for entry in entries:
        entry = entry.strip()
        instance = entry.split("\t")[0]
        cluster = entry.split("\t")[2]

        try:
            lemma = instance.split(".")[0] + "." + instance.split(".")[1]
        except:
            print(instance)

        if instance not in record.keys():
            record[instance] = {}
        
        if "lemma" not in record[instance].keys():
            record[instance]["lemma"] = lemma

        if "senses" not in record[instance].keys():
            record[instance]["senses"] = {}

        if clusters[cluster][0] not in record[instance]["senses"].keys():
            record[instance]["senses"][clusters[cluster][0]] = 0

        record[instance]["senses"][clusters[cluster][0]] += 1

    keyword = file.split(".")[0] + "." +file.split(".")[1]

    if keyword not in records.keys():
        records[keyword] = record
    

for key, val in records.items():
    with open(os.path.join(path, "all.me.key"), "a") as fp:
        for k, v in val.items():
            instance_id = k
            lemma = v["lemma"]
            senses = []
            for kk, vv in v["senses"].items():
                if vv < 5:
                    continue
                else:
                    senses.append(kk + "/" + str(vv))
            fp.write(lemma + " " + instance_id + " " + " ".join(senses) + "\n")
    fp.close()
        
            
       