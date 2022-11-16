import sys
import matplotlib.image as img
import pandas as pd

if(len(sys.argv) >= 2):
    imageMat = img.imread(sys.argv[1])
else:
    exit(1)

if(imageMat.shape[2] == 3):
    imageMat_reshape = imageMat.reshape(imageMat.shape[0], -1)
else:
    imageMat_reshape = imageMat

mat_df = pd.DataFrame(imageMat_reshape)

mat_df.to_csv('image.csv',
              header=None,
              index=None)

loaded_df = pd.read_csv('image.csv',
                        sep=',',
                        header=None)
loaded_2D_mat = loaded_df.values

loaded_mat = loaded_2D_mat.reshape(loaded_2D_mat.shape[0],
                                   loaded_2D_mat.shape[1] // imageMat.shape[2],
                                   imageMat.shape[2])

if((imageMat == loaded_mat).all()):
    print("Successfully converted.")
