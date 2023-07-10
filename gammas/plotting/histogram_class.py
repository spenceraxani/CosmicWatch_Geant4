import numpy as np

class Histogram(object):

	def __init__(self, bin_centers, freq):
		self.bin_centers = bin_centers
		self.nbins = len(bin_centers)
		self.freq = freq
		self.norm_freq = None
		self.fmax = None
		self.mean = None
		self.sigma = None

		#errors of each value
		self.freq_err = None
		self.norm_freq_err = None
		self.fmax_err = None
		self.mean_err = None
		#self.sigma_err = None		

	def getMean(self):
		#total events recorded
		total_events = sum(self.freq)

		#bin center * it's frecuency
		rel_freq = sum(f*c for f, c in zip(self.freq, self.bin_centers))

		self.mean = round(rel_freq/total_events, 2)

	def getSigma(self):
		if self.mean is None:
			self.getMean()

		#sum of deviations from the mean
		differences = sum((c-self.mean)**2 for c in self.bin_centers)

		self.sigma = round(np.sqrt(differences/self.nbins), 2)

	def normalize(self):
		if self.norm_freq is not None:
			print("Already normalized")

		self.fmax = max(self.freq)

		self.norm_freq = self.freq/self.fmax

	def getErrors(self):
		if self.norm_freq is None:
			self.normalize()

		self.fmax_err = np.sqrt(self.fmax)/self.fmax #relative error at fmax bin

		self.freq_err = [np.sqrt(f)/f if f else 0 for f in self.freq] #relative error at each bin

		self.norm_freq_err = [0]*self.nbins #frequencies error in the normalized histogram
		for j in range(self.nbins):
			self.norm_freq_err[j] = (self.freq[j]/self.fmax)*np.sqrt(self.freq_err[j]**2 + self.fmax_err**2)

		self.mean_err = round(self.sigma/np.sqrt(self.nbins), 2)