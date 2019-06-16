import pandas as pd

df = pd.read_csv("cores.csv")

columns = df.columns.tolist()
df.drop('Name_under', axis = 1, inplace = True)
df.drop('Number', axis = 1, inplace = True)


print(df.head())


# columns = [columns[1] , columns[0] , columns[2] , columns[3]]
# df.sort_values(by=['Brand'], inplace=True)

# df = df[columns]
df.to_csv('cores.csv', index = False)
