import CoreBluetooth
import UIKit

var myPeripheal: CBPeripheral?
var myCharacteristic: CBCharacteristic?
var manager: CBCentralManager?

let serviceUUID = CBUUID(string: "4fafc201-1fb5-459e-8fcc-c5c9c331914b")
let periphealUUID = CBUUID(string: "8F917D8A-2EEF-770C-A671-EEC466375E56")

class ViewController: UIViewController, CBCentralManagerDelegate {
    @IBOutlet var connectButton: UIButton!
    @IBOutlet var sendText1Button: UIButton!
    @IBOutlet var sendText2Button: UIButton!
    @IBOutlet var sendImageButton: UIButton!
    @IBOutlet var disconnectButton: UIButton!

    override func viewDidLoad() {
        super.viewDidLoad()
        manager = CBCentralManager(delegate: self, queue: nil)
    }

    @IBAction func scanButtonTouched(_ sender: Any) {
        manager?.stopScan()
        // manager?.scanForPeripherals(withServices: nil)
        manager?.scanForPeripherals(withServices: [serviceUUID], options: nil)
    }

    @IBAction func sendText1Touched(_ sender: Any) {
        sendText(text: "AHeyHo")
    }

    @IBAction func sendText2Touched(_ sender: Any) {
        sendText(text: "B")
    }

    @IBAction func disconnectTouched(_ sender: Any) {
        manager?.cancelPeripheralConnection(myPeripheal!)
    }

    @IBAction func selectPhotoPressed(_ sender: Any) {
        let imagePickerVC = UIImagePickerController()
        imagePickerVC.sourceType = .photoLibrary
        imagePickerVC.delegate = self
        present(imagePickerVC, animated: true)
    }

    func sendText(text: String) {
        if myPeripheal != nil && myCharacteristic != nil {
            let data = text.data(using: .utf8)
            myPeripheal!.writeValue(data!, for: myCharacteristic!, type: CBCharacteristicWriteType.withResponse)
        }
    }

    func convertImage(_ image: UIImage) -> Data {
        let grayScale: [UInt16] = image.pixelData.map {
            var white: CGFloat = 0
            var alpha: CGFloat = 0
            $0.color.getWhite(&white, alpha: &alpha)
            return white > 0.5 ? 1 : 0
        }
        let data = grayScale.data
        return data
    }

    func sendImage(image: UIImage) {
        let options: NSDictionary = [:]
        if let myPeripheal = myPeripheal,
           let myCharacteristic = myCharacteristic,
           let monoImage = image.monochrome {
            let colors = convertImage(monoImage)
            myPeripheal.writeValue(colors, for: myCharacteristic, type: CBCharacteristicWriteType.withResponse)
        }
    }

    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String: Any], rssi RSSI: NSNumber) {
        print("UUID: \(peripheral.identifier.uuidString)")
        if peripheral.identifier.uuidString == periphealUUID.uuidString {
            myPeripheal = peripheral
            myPeripheal?.delegate = self
            manager?.connect(myPeripheal!, options: nil)
            manager?.stopScan()
        }
    }

    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .poweredOff:
            print("Bluetooth is switched off")
        case .poweredOn:
            print("Bluetooth is switched on")
        case .unsupported:
            print("Bluetooth is not supported")
        default:
            print("Unknown state")
        }
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        peripheral.discoverServices([serviceUUID])
        print("Connected to " + peripheral.name!)

        connectButton.isEnabled = false
        disconnectButton.isEnabled = true
        sendText1Button.isHidden = false
        sendText2Button.isHidden = false
        sendImageButton.isHidden = false
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        print("Disconnected from " + peripheral.name!)

        myPeripheal = nil
        myCharacteristic = nil

        connectButton.isEnabled = true
        disconnectButton.isEnabled = false
        sendText1Button.isHidden = true
        sendText2Button.isHidden = true
        sendImageButton.isHidden = true
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        print(error!)
    }
}

extension ViewController: CBPeripheralDelegate {
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else { return }

        for service in services {
            peripheral.discoverCharacteristics(nil, for: service)
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let characteristics = service.characteristics else { return }
        myCharacteristic = characteristics[0]
    }
}

extension ViewController: UIImagePickerControllerDelegate & UINavigationControllerDelegate {
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey: Any]) {
        picker.dismiss(animated: true, completion: nil)

        if let image = info[.originalImage] as? UIImage {
            sendImage(image: image)
        }
    }
}

extension Array {
    var data: Data { withUnsafeBytes { .init($0) } }
}
