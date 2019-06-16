import pandas as pd

df = pd.read_csv("cidades.csv")

columns = df.columns.tolist()
print(columns)

columns = [columns[1] , columns[0] , columns[2] , columns[3]]
# df.sort_values('2')

df = df[columns]
df.to_csv('cidades.csv', index = False)
