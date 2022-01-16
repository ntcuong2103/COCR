import argparse
from pathlib import Path

import tensorflow as tf
from tensorflow import keras

from utils import Decoder

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--images', type=str, required=True,
                    help='Image file or folder path.')
parser.add_argument('-t', '--table_path', type=str, default='./old_table.txt',
                    help='The path of table file.')
# parser.add_argument('-w', '--img_width', type=int, default=32,
#                     help='Image width, this parameter will affect the output '
#                          'shape of the model, default is 100, so this model '
#                          'can only predict up to 24 characters.')
parser.add_argument('--img_channels', type=int, default=1,
                    help='0: Use the number of channels in the image, '
                         '1: Grayscale image, 3: RGB image')
parser.add_argument('-m', '--model', type=str, default='./old_weights.h5',
                    help='The saved model.')
args = parser.parse_args()


def read_img_and_preprocess(path):
    img = tf.io.read_file(path)
    img = tf.io.decode_jpeg(img, channels=args.img_channels)
    img = tf.image.convert_image_dtype(img, tf.float32)
    w, h, _ = img.shape
    img = tf.image.resize(img, (32, 32 * w // h))
    return img


p = Path(args.images)
if p.is_dir():
    img_paths = p.iterdir()
    imgs = [read_img_and_preprocess(str(x)) for x in p.iterdir()]
    imgs = tf.stack(imgs)
else:
    img_paths = [p]
    img = read_img_and_preprocess(str(p))
    imgs = tf.expand_dims(img, 0)

with open(args.table_path, 'r') as f:
    inv_table = [char.strip() for char in f]

model = keras.models.load_model(args.model, compile=False)

decoder = Decoder(inv_table)

y_pred = model(imgs)
for path, g_pred, b_pred in zip(img_paths,
                                decoder.decode(y_pred, method='greedy'),
                                decoder.decode(y_pred, method='beam_search')):
    print('Path: {}, greedy: {}, beam search: {}'.format(path, g_pred, b_pred))
