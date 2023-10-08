import parselmouth
from parselmouth.praat import call

sound = parselmouth.Sound("speak.mp3")
manipulation = call(sound, "To Manipulation", 0.01, 75, 600)

pitch_tier = call(manipulation, "Extract pitch tier")

# 0.5 here gives a lower octave 
call(pitch_tier, "Multiply frequencies", sound.xmin, sound.xmax, 0.5)

call([pitch_tier, manipulation], "Replace pitch tier")
sound_octave_up = call(manipulation, "Get resynthesis (overlap-add)")

sound_octave_up.save("4_b_octave_down.wav", "WAV")
