import pandas as pd
from flask import Flask, request, render_template, session, redirect
# pd.set_option('display.max_columns', None)
# pd.set_option('display.max_rows', None)

from pandasgui import show
LOG_PATH = '../logs/all_quads.json'
df = pd.read_json(LOG_PATH)
show(df)

# app = Flask(__name__)


from pandasgui import show
#
# @app.route('/', methods=("POST", "GET"))
# def html_table():
#     df = pd.read_json(LOG_PATH)
#     return render_template('df.html',
#                            tables=[df.to_html(classes='data', index=False)],
#                            titles=df.columns.values)
#
#
# if __name__ == '__main__':
#     app.run(host='0.0.0.0')
