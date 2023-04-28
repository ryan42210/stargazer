import math
import random

def DegToRad(a):
    return a * math.pi / 180

# convert ra and dec (in deg) to unit sphere cartesian coordinate
def SphereToCartesian(ra, dec):
    x = math.cos(DegToRad(ra)) * math.cos(DegToRad(dec))
    y = math.sin(DegToRad(ra)) * math.cos(DegToRad(dec))
    z = math.sin(DegToRad(dec))
    return (x, y, z)

def GenStars():
    ra_gen = random.uniform(0, 30)
    dec_gen = random.uniform(0, 30)
    return (ra_gen, dec_gen)


