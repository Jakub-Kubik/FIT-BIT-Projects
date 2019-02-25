from django.db import models
from django.contrib.auth import get_user_model

User = get_user_model()


# Create your models here.

# class AboutProduct(models.Model):
#    product = models.ForeignKey('Products', models.DO_NOTHING, primary_key=True)
#   zaznam = models.ForeignKey('Zaznamy', models.DO_NOTHING)

class Products(models.Model):
    product_id = models.AutoField(primary_key=True)
    deprecation = models.CharField(max_length=20, blank=True, null=True)  # opotrebenie
    assembly_date = models.DateField(blank=True, null=True)
    # products.Costumes.product: (fields.W342) Setting unique=True on a ForeignKey has the same effect as using a OneToOneField.
    manager = models.ForeignKey(User, on_delete=models.CASCADE, blank=True, null=True)
    is_costume = models.BooleanField(default=True)


# class ProductsTogether(models.Model):
#    costume = models.ForeignKey(Costumes, models.DO_NOTHING, primary_key=True)
#    accessory = models.ForeignKey(Accessory, models.DO_NOTHING)
#

class Accessory(models.Model):
    product = models.OneToOneField(Products, on_delete=models.CASCADE, primary_key=True)
    name = models.CharField(max_length=32, blank=True, null=True)
    description = models.CharField(max_length=128, blank=True, null=True)
    amount = models.PositiveIntegerField(default=0)
    price = models.FloatField(null=True, blank=True, default=None)


class Costumes(models.Model):
    product = models.OneToOneField(Products, on_delete=models.CASCADE, primary_key=True)
    name = models.CharField(max_length=32, blank=True, null=True)
    description = models.CharField(max_length=128)
    manufacturer = models.CharField(max_length=32, blank=True, null=True)
    fabric = models.CharField(max_length=32, blank=True, null=True)  # material
    color = models.CharField(max_length=16, blank=True, null=True)
    size = models.CharField(max_length=16)
    category = models.CharField(max_length=16, blank=True, null=True)
    amount = models.PositiveIntegerField(default=0)
    price = models.FloatField(null=True, blank=True, default=None)
