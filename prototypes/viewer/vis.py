import pandas as pd
import dtale
LOG_PATH = '../logs/all_quads.json'
df = pd.read_json(LOG_PATH)
dtale.show(df)
